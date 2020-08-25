#include "PlatformAudioAndroid.h"

#include "PlatformException.h"

#include <webrtc/sdk/android/native_api/jni/jvm.h>

#include <string>

using namespace virgil::voip;
using namespace std::literals;


PlatformAudioAndroid::PlatformAudioAndroid()
    : javaPlatformAudio_(std::make_unique<PlatformAudioAndroid::JavaPlatformAudio>()) {
}

bool
PlatformAudioAndroid::hasSpeaker() const {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->hasSpeaker();
}

bool
PlatformAudioAndroid::setSpeakerOn() {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->setSpeakerOn();
}

bool
PlatformAudioAndroid::setSpeakerOff() {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->setSpeakerOff();
}

bool
PlatformAudioAndroid::hasMicrophone() const {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->hasMicrophone();
}

bool
PlatformAudioAndroid::setMicrophoneOn() {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->setMicrophoneOn();
}

bool
PlatformAudioAndroid::setMicrophoneOff() {
    rtc::CritScope lock(&javaPlatformAudioMutex_);
    return javaPlatformAudio_->setMicrophoneOff();
}

PlatformAudioAndroid::JavaPlatformAudio::JavaPlatformAudio() {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(env, PlatformError::FailedJNI);

    constexpr const char *className = "com/virgilsecurity/voip/PlatformAudio";

    try {
        jclass platformAudioClass = env->FindClass(className);
        PlatformException::throwIfTrue(
                (nullptr == platformAudioClass) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className);

        jmethodID platformAudioCtor = env->GetMethodID(platformAudioClass, "<init>", "()V");
        PlatformException::throwIfTrue(
                (nullptr == platformAudioCtor) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " constructor"s);

        hasSpeakerMethod_ = env->GetMethodID(platformAudioClass, "hasSpeaker", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == hasSpeakerMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: hasSpeaker()"s);

        setSpeakerOnMethod_ = env->GetMethodID(platformAudioClass, "setSpeakerOn", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == setSpeakerOnMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: setSpeakerOn()"s);

        setSpeakerOffMethod_ = env->GetMethodID(platformAudioClass, "setSpeakerOff", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == setSpeakerOffMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: setSpeakerOff()"s);

        hasMicrophoneMethod_ = env->GetMethodID(platformAudioClass, "hasMicrophone", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == hasMicrophoneMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: hasMicrophone()"s);

        setMicrophoneOnMethod_ = env->GetMethodID(platformAudioClass, "setMicrophoneOn", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == setMicrophoneOnMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: setMicrophoneOn()"s);

        setMicrophoneOffMethod_ = env->GetMethodID(platformAudioClass, "setMicrophoneOff", "()Z");
        PlatformException::throwIfTrue(
                (nullptr == setMicrophoneOffMethod_) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to load java class: "s + className + " method: setMicrophoneOff()"s);


        jobject platformAudio = env->NewObject(platformAudioClass, platformAudioCtor);
        PlatformException::throwIfTrue(
                (nullptr == platformAudio) || env->ExceptionCheck(),
                PlatformError::FailedJNI,
                "Failed to create java class: "s + className);

        platformAudio_ = env->NewGlobalRef(platformAudio);
    } catch (...) {
        env->ExceptionClear();
        throw;
    }
}

PlatformAudioAndroid::JavaPlatformAudio::~JavaPlatformAudio() noexcept {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    if (env) {
        env->DeleteGlobalRef(platformAudio_);
    } else {
        // TODO: Log memory leak.
    }
}

bool
PlatformAudioAndroid::JavaPlatformAudio::hasSpeaker() const {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, hasSpeakerMethod_);
}

bool
PlatformAudioAndroid::JavaPlatformAudio::setSpeakerOn() {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, setSpeakerOnMethod_);
}

bool
PlatformAudioAndroid::JavaPlatformAudio::setSpeakerOff() {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, setSpeakerOffMethod_);
}

bool
PlatformAudioAndroid::JavaPlatformAudio::hasMicrophone() const {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, hasMicrophoneMethod_);
}

bool
PlatformAudioAndroid::JavaPlatformAudio::setMicrophoneOn() {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, setMicrophoneOnMethod_);
}

bool
PlatformAudioAndroid::JavaPlatformAudio::setMicrophoneOff() {
    JNIEnv *jni = webrtc::AttachCurrentThreadIfNeeded();
    PlatformException::throwIfNull(jni, PlatformError::FailedJNI);

    return jni->CallBooleanMethod(platformAudio_, setMicrophoneOffMethod_);
}
