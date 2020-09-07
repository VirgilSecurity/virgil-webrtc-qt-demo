#include "PlatformAudioIOS.h"

#include "PlatformException.h"
#include "ScopeGuard.h"

using namespace virgil::voip;

static void
throwIfError(NSError *error) {
    if (error) {
        NSString *description = [error localizedDescription];
        const char *descriptionCStr = [description cStringUsingEncoding:NSASCIIStringEncoding];
        throw PlatformException(PlatformError::FailedPlatformAudio, descriptionCStr);
    }
}

PlatformAudioIOS::PlatformAudioIOS() : rtcAudioSession_([RTC_OBJC_TYPE(RTCAudioSession) sharedInstance]) {
}

bool
PlatformAudioIOS::hasSpeaker() const {
    return true;
}

void
PlatformAudioIOS::setSpeakerOn(bool on) {
    [rtcAudioSession_ lockForConfiguration];

    auto _ = ScopeGuard([this] {
        [rtcAudioSession_ unlockForConfiguration];
    });

#if DEBUG
    NSLog(@"PlatformAudioIOS: set speaker %@.", on ? @"on" : @"off");
#endif

    NSError *error = NULL;

    [rtcAudioSession_ setCategory:AVAudioSessionCategoryPlayAndRecord
                      withOptions:AVAudioSessionCategoryOptionInterruptSpokenAudioAndMixWithOthers
                            error:&error];

    throwIfError(error);

    const auto outputAudioPort = on ? AVAudioSessionPortOverrideSpeaker : AVAudioSessionPortOverrideNone;
    const auto isSuccess = [rtcAudioSession_ overrideOutputAudioPort:outputAudioPort error:&error];

    throwIfError(error);
    PlatformException::throwIfFalse(isSuccess, PlatformError::FailedPlatformAudio, "Failed to configure speaker.");
}

std::unique_ptr<PlatformAudio>
PlatformAudio::createDefault() {
    return std::make_unique<PlatformAudioIOS>();
}
