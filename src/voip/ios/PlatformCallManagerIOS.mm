#include "PlatformCallManagerIOS.h"

#include "PlatformException.h"
#include "ScopeGuard.h"

#include <webrtc/sdk/objc/components/audio/RTCAudioSession.h>

#include <atomic>

#import <Foundation/Foundation.h>
#import <CallKit/CallKit.h>


using namespace virgil::voip;

// --------------------------------------------------------------------------
//  Objective-C/C++ helpers.
// --------------------------------------------------------------------------
namespace objc {

std::string
to_utf8(NSString *str) {
    return std::string{[str cStringUsingEncoding:NSUTF8StringEncoding]};
}

NSString *
to_utf8(const std::string &str) {
    return [NSString stringWithUTF8String:str.c_str()];
}

} // namespace objc

// --------------------------------------------------------------------------
//  Route CXProviderDelegate -> to the PlatformCallManager signals.
// --------------------------------------------------------------------------
@interface __CallDelegate : NSObject <CXProviderDelegate>
@property(nonatomic, readonly) PlatformCallManagerIOS *_Nonnull platformCallManager;
@property(nonatomic, readonly) dispatch_queue_t queue;
@end

@implementation __CallDelegate
- (id)initWithThis:(PlatformCallManagerIOS *_Nonnull)platformCallManager {
    if (self = [super init]) {
        _platformCallManager = platformCallManager;
        _queue = dispatch_queue_create("call-kit-events-queue", DISPATCH_QUEUE_SERIAL);

        __weak __CallDelegate *weakSelf = self;
        [[NSNotificationCenter defaultCenter] addObserverForName:@"callFromTheCallList"
                                                          object:nil
                                                           queue:nil
                                                      usingBlock:^(NSNotification *notification) {
                                                        NSString *callee = notification.object;
                                                        NSUUID *callUUID = [[NSUUID alloc] init];
                                                        weakSelf.platformCallManager->didRequestCallStart(
                                                                objc::to_utf8(callUUID.UUIDString.lowercaseString),
                                                                objc::to_utf8(callee));
                                                      }];

        return self;
    }

    return nil;
}

- (void)providerDidReset:(CXProvider *)provider {
    (void)provider;
    self.platformCallManager->didRequestReset();
}

- (void)providerDidBegin:(CXProvider *)provider {
    (void)provider;
    self.platformCallManager->isReady();
}

- (void)provider:(CXProvider *)provider performStartCallAction:(CXStartCallAction *)action {
    (void)provider;
    std::string callUUID = objc::to_utf8(action.callUUID.UUIDString.lowercaseString);
    self.platformCallManager->didStartOutgoingCall(callUUID);
    [action fulfill];
}

- (void)provider:(CXProvider *)provider performAnswerCallAction:(CXAnswerCallAction *)action {
    (void)provider;
    std::string callUUID = objc::to_utf8(action.callUUID.UUIDString.lowercaseString);
    self.platformCallManager->didRequestCallAnswer(callUUID);
    [action fulfill];
}

- (void)provider:(CXProvider *)provider performEndCallAction:(CXEndCallAction *)action {
    (void)provider;
    std::string callUUID = objc::to_utf8(action.callUUID.UUIDString.lowercaseString);
    self.platformCallManager->didRequestCallEnd(callUUID);
    self.platformCallManager->didEndCall(callUUID);
    [action fulfill];
}

- (void)provider:(CXProvider *)provider performSetHeldCallAction:(CXSetHeldCallAction *)action {
    (void)provider;
    std::string callUUID = objc::to_utf8(action.callUUID.UUIDString.lowercaseString);
    self.platformCallManager->didRequestCallHold(callUUID, action.onHold);
    [action fulfill];
}

- (void)provider:(CXProvider *)provider performSetMutedCallAction:(CXSetMutedCallAction *)action {
    (void)provider;
    std::string callUUID = objc::to_utf8(action.callUUID.UUIDString.lowercaseString);
    self.platformCallManager->didRequestCallMute(callUUID, action.isMuted);
    [action fulfill];
}
@end

// --------------------------------------------------------------------------
//  Singleton.
// --------------------------------------------------------------------------
PlatformCallManager &
PlatformCallManager::sharedInstance() {
    static PlatformCallManagerIOS instance{};
    return instance;
}

// --------------------------------------------------------------------------
//  PIMPL
// --------------------------------------------------------------------------
class PlatformCallManagerIOS::Impl {
public:
    RTC_OBJC_TYPE(RTCAudioSession) *audioSession = nullptr;
    CXProvider *callProvider = nullptr;
    CXCallController *callController = nullptr;
    __CallDelegate *callDelegate = nullptr;
    std::atomic_bool isRegistered = false;
};

// --------------------------------------------------------------------------
//  PlatformCallManagerIOS
// --------------------------------------------------------------------------
PlatformCallManagerIOS::PlatformCallManagerIOS() : impl_(std::make_unique<Impl>()) {
}

void
PlatformCallManagerIOS::tellSystemRegisterApplication(const std::string &appName) {
    bool isRegisteredExpected = false;
    if (!impl_->isRegistered.compare_exchange_strong(isRegisteredExpected, true)) {
        throw PlatformException(
                PlatformError::FailedPlatformCallManager,
                "PlatformCallManagerIOS: application was already registered.");
    }

    //
    //  Create RTCAudioSession.
    //
    impl_->audioSession = [RTC_OBJC_TYPE(RTCAudioSession) sharedInstance];

    //
    //  Create CXProvider.
    //
    auto configuration = [[CXProviderConfiguration alloc] initWithLocalizedName:objc::to_utf8(appName)];

    configuration.supportsVideo = YES;
    configuration.maximumCallsPerCallGroup = 1;
    configuration.supportedHandleTypes = [NSSet setWithObject:@(CXHandleTypePhoneNumber)];

    impl_->callDelegate = [[__CallDelegate alloc] initWithThis:this];
    impl_->callProvider = [[CXProvider alloc] initWithConfiguration:configuration];
    [impl_->callProvider setDelegate:impl_->callDelegate queue:impl_->callDelegate.queue];

    //
    //  Create CXCallController.
    //
    impl_->callController = [[CXCallController alloc] init];
}

void
PlatformCallManagerIOS::tellSystemStartIncomingCall(const std::string &callUUID, const std::string &caller) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);

    auto uuid = [[NSUUID alloc] initWithUUIDString:objc::to_utf8(callUUID)];
    auto handle = [[CXHandle alloc] initWithType:CXHandleTypePhoneNumber value:objc::to_utf8(caller)];

    auto update = [[CXCallUpdate alloc] init];
    update.remoteHandle = handle;

    std::weak_ptr<Impl> weakImpl{impl_};
    [impl_->callProvider
            reportNewIncomingCallWithUUID:uuid
                                   update:update
                               completion:^(NSError *_Nullable error) {
                                 if (auto impl = weakImpl.lock()) {
                                     auto callUUID = objc::to_utf8(uuid.UUIDString.lowercaseString);
                                     if (!error) {
                                         impl->callDelegate.platformCallManager->didStartIncomingCall(callUUID);

                                     } else {
                                         auto description = objc::to_utf8([error localizedDescription]);
                                         impl->callDelegate.platformCallManager->didFailStartCall(
                                                 callUUID,
                                                 description);
                                     }
                                 }
                               }];
}

void
PlatformCallManagerIOS::tellSystemStartOutgoingCall(const std::string &callUUID, const std::string &callee) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);

    auto uuid = [[NSUUID alloc] initWithUUIDString:objc::to_utf8(callUUID)];
    auto handle = [[CXHandle alloc] initWithType:CXHandleTypePhoneNumber value:objc::to_utf8(callee)];

    auto startCallAction = [[CXStartCallAction alloc] initWithCallUUID:uuid handle:handle];
    auto transaction = [[CXTransaction alloc] init];
    [transaction addAction:startCallAction];

    std::weak_ptr<Impl> weakImpl{impl_};
    [impl_->callController
            requestTransaction:transaction
                    completion:^(NSError *_Nullable error) {
                      if (auto impl = weakImpl.lock()) {
                          auto callUUID = objc::to_utf8(uuid.UUIDString.lowercaseString);
                          if (error) {
                              auto description = objc::to_utf8([error localizedDescription]);
                              impl->callDelegate.platformCallManager->didFailStartCall(callUUID, description);
                          }
                      }
                    }];
}

void
PlatformCallManagerIOS::tellSystemCallConnected(const std::string &callUUID) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);
}

void
PlatformCallManagerIOS::tellSystemEndCall(const std::string &callUUID) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);

    auto uuid = [[NSUUID alloc] initWithUUIDString:objc::to_utf8(callUUID)];

    auto endCallAction = [[CXEndCallAction alloc] initWithCallUUID:uuid];
    auto transaction = [[CXTransaction alloc] init];
    [transaction addAction:endCallAction];
    std::weak_ptr<Impl> weakImpl{impl_};
    [impl_->callController requestTransaction:transaction
                                   completion:^(NSError *_Nullable error) {
                                     if (auto impl = weakImpl.lock()) {
                                         auto callUUID = objc::to_utf8(uuid.UUIDString.lowercaseString);
                                         impl->callDelegate.platformCallManager->didEndCall(callUUID);
                                         if (error) {
                                             NSLog(@"PlatformCallManagerIOS: Error: %@ %@", error, [error userInfo]);
                                         }
                                     }
                                   }];
}

void
PlatformCallManagerIOS::tellSystemDummyIncomingCall() {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);
}

void
PlatformCallManagerIOS::tellSystemMuteCall(const std::string &callUUID, bool onMute) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);
}

void
PlatformCallManagerIOS::tellSystemHoldCall(const std::string &callUUID, bool onHold) {
    PlatformException::throwIfFalse(impl_->isRegistered, PlatformError::PlatformCallManager_ApplicationIsNotRegistered);
}
