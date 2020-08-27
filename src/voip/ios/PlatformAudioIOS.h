#ifndef VIRGIL_VOIP_PLATFORM_AUDIO_IOS_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_AUDIO_IOS_H_INCLUDED

#include "PlatformAudio.h"

#include <webrtc/sdk/objc/components/audio/RTCAudioSession.h>

#include <memory>

namespace virgil {
namespace voip {

//
// Implementation for iOS.
//
class PlatformAudioIOS : public PlatformAudio {
public:
    PlatformAudioIOS();

    //
    // Speaker control.
    //
    virtual bool
    hasSpeaker() const override;

    virtual void
    setSpeakerOn(bool on) override;

private:
    RTC_OBJC_TYPE(RTCAudioSession) * rtcAudioSession_;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_AUDIO_IOS_H_INCLUDED */
