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
// Implementation based on the WebRTC class "AudioDeviceModule".
//
class PlatformAudioAndroid : public PlatformAudio {
public:
    PlatformAudioAndroid();

    //
    // Speaker control.
    //
    virtual bool
    hasSpeaker() const override;

    virtual bool
    setSpeakerOn() override;

    virtual bool
    setSpeakerOff() override;

    //
    // Microphone control.
    //
    virtual bool
    hasMicrophone() const override;

    virtual bool
    setMicrophoneOn() override;

    virtual bool
    setMicrophoneOff() override;


private:
    //
    // Wraps the corresponding Java class.
    //
    class JavaPlatformAudio {
    public:
        JavaPlatformAudio();
        ~JavaPlatformAudio() noexcept;

        bool
        hasSpeaker() const;

        bool
        setSpeakerOn();

        bool
        setSpeakerOff();

        bool
        hasMicrophone() const;

        bool
        setMicrophoneOn();

        bool
        setMicrophoneOff();


    private:
        jobject platformAudio_;
        jmethodID hasSpeakerMethod_;
        jmethodID setSpeakerOnMethod_;
        jmethodID setSpeakerOffMethod_;
        jmethodID hasMicrophoneMethod_;
        jmethodID setMicrophoneOnMethod_;
        jmethodID setMicrophoneOffMethod_;
    };

    mutable rtc::CriticalSection javaPlatformAudioMutex_;
    std::unique_ptr<JavaPlatformAudio> javaPlatformAudio_ RTC_GUARDED_BY(javaPlatformAudioMutex_);
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_AUDIO_ANDROID_H_INCLUDED */
