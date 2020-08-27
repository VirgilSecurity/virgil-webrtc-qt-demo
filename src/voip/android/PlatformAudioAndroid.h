#ifndef VIRGIL_VOIP_PLATFORM_AUDIO_ANDROID_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_AUDIO_ANDROID_H_INCLUDED

#include "PlatformAudio.h"

#include <webrtc/rtc_base/critical_section.h>
#include <webrtc/rtc_base/thread_annotations.h>

#include <jni.h>

#include <memory>

namespace virgil {
namespace voip {

//
// Implementation for Android.
//
class PlatformAudioAndroid : public PlatformAudio {
public:
    PlatformAudioAndroid();
    ~PlatformAudioAndroid() noexcept;

    //
    // Speaker control.
    //
    virtual bool
    hasSpeaker() const override;

    virtual void
    setSpeakerOn(bool on) override;

private:
    mutable rtc::CriticalSection jPlatformAudioMutex_;
    mutable jobject jPlatformAudio_ RTC_GUARDED_BY(jPlatformAudioMutex_) = nullptr;
    jmethodID jHasSpeakerMethod_ = nullptr;
    jmethodID jSetSpeakerOnMethod_ = nullptr;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_AUDIO_ANDROID_H_INCLUDED */
