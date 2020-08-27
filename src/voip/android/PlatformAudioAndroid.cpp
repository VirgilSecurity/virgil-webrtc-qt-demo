#include "PlatformAudioAndroid.h"

#include "PlatformException.h"

#include <webrtc/sdk/android/native_api/jni/jvm.h>

#include <string>

using namespace virgil::voip;
using namespace std::literals;


PlatformAudioAndroid::PlatformAudioAndroid() {
    JNIEnv *jEnv = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jEnv, PlatformError::FailedJNI);

    constexpr const char *className = "com/virgilsecurity/voip/PlatformAudio";

    try {
        jclass jPlatformAudioClass = jEnv->FindClass(className);
        PlatformException::throwIfTrue(
                (nullptr == jPlatformAudioClass) || jEnv->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className);

        jmethodID jPlatformAudioCtor = jEnv->GetMethodID(jPlatformAudioClass, "<init>", "()V");
        PlatformException::throwIfTrue(
                (nullptr == jPlatformAudioCtor) || jEnv->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " constructor"s);

        jHasSpeakerMethod_ = jEnv->GetMethodID(jPlatformAudioClass, "hasSpeaker", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == jHasSpeakerMethod_) || jEnv->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + ", method: hasSpeaker()"s);

        jSetSpeakerOnMethod_ = jEnv->GetMethodID(jPlatformAudioClass, "setSpeakerOn", "(Z)V");
        PlatformException::throwIfTrue(
                (nullptr == jSetSpeakerOnMethod_) || jEnv->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + ", method: setSpeakerOn()"s);

        jobject jPlatformAudio = jEnv->NewObject(jPlatformAudioClass, jPlatformAudioCtor);
        PlatformException::throwIfTrue(
                (nullptr == jPlatformAudio) || jEnv->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to create java class: "s + className);

        jPlatformAudio_ = jEnv->NewGlobalRef(jPlatformAudio);
    } catch (...) {
        jEnv->ExceptionClear();
        throw;
    }
}

PlatformAudioAndroid::~PlatformAudioAndroid() noexcept {
    JNIEnv *jEnv = webrtc::AttachCurrentThreadIfNeeded();
    if (jEnv) {
        jEnv->DeleteGlobalRef(jPlatformAudio_);
    } else {
        // TODO: Log memory leak.
    }
}

bool
PlatformAudioAndroid::hasSpeaker() const {
    rtc::CritScope lock(&jPlatformAudioMutex_);

    JNIEnv *jEnv = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jEnv, PlatformError::FailedJNI);

    return jEnv->CallBooleanMethod(jPlatformAudio_, jHasSpeakerMethod_);
}

void
PlatformAudioAndroid::setSpeakerOn(bool on) {
    rtc::CritScope lock(&jPlatformAudioMutex_);

    JNIEnv *jEnv = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jEnv, PlatformError::FailedJNI);

    jEnv->CallVoidMethod(jPlatformAudio_, jSetSpeakerOnMethod_, on);
}

std::unique_ptr<PlatformAudio>
PlatformAudio::createDefault() {
    return std::make_unique<PlatformAudioAndroid>();
}
