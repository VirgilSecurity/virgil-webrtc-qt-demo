#import "UIKit/UIKit.h"

#import <Intents/Intents.h>

#include <QDebug>

@interface QIOSApplicationDelegate
@end

@interface QIOSApplicationDelegate (VirgilWebRTCApplicationDelegate)
@end

@implementation QIOSApplicationDelegate (VirgilWebRTCApplicationDelegate)

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    (void)application;
    (void)launchOptions;
    qDebug() << "AppDelegate:application:didFinishLaunchingWithOptions";
    return YES;
}

- (BOOL)application:(UIApplication *)application
        continueUserActivity:(NSUserActivity *)userActivity
          restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> *restorableObjects))restorationHandler {

    (void)application;
    (void)restorationHandler;
    qDebug() << "AppDelegate:application:continueUserActivity:restorationHandler";

    INInteraction *interaction = userActivity.interaction;
    INPerson *contact = nil;
    NSString *handle = nil;
    BOOL isAudioCall = false;
    BOOL isVideoCall = false;

    // iOS 13 returns an INStartCallIntent userActivity type
    if (@available(iOS 13, *)) {
        INStartCallIntent *intent = (INStartCallIntent *)interaction.intent;
        // callCapability is not available on iOS > 13.2, but it is in 13.1 weirdly...
        if ([intent respondsToSelector:@selector(callCapability)]) {
            isAudioCall = intent.callCapability == INCallCapabilityAudioCall;
            isVideoCall = intent.callCapability == INCallCapabilityVideoCall;
        } else {
            isAudioCall = [userActivity.activityType isEqualToString:INStartAudioCallIntentIdentifier];
            isVideoCall = [userActivity.activityType isEqualToString:INStartVideoCallIntentIdentifier];
        }
    }

    if (isAudioCall) {
        INStartAudioCallIntent *startAudioCallIntent = (INStartAudioCallIntent *)interaction.intent;
        contact = [startAudioCallIntent.contacts firstObject];
    } else if (isVideoCall) {
        INStartVideoCallIntent *startVideoCallIntent = (INStartVideoCallIntent *)interaction.intent;
        contact = [startVideoCallIntent.contacts firstObject];
    }

    if (contact != nil) {
        handle = contact.personHandle.value;
    }

    if (handle != nil && handle.length > 0) {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"callFromTheCallList" object:handle];
        qDebug() << "Try to make outgoing call from the contact list to: " << handle;
        return YES;
    }

    return NO;
}

@end
