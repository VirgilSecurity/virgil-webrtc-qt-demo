#include "PlatformAudioWebRtc.h"

#include <stdexcept>

using namespace virgil::voip;

PlatformAudioWebRtc::PlatformAudioWebRtc(rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule)
    : audioDeviceModule_(std::move(audioDeviceModule)) {

    if (!audioDeviceModule_) {
        throw std::logic_error("PlatformAudioWebRtc: audioDeviceModule is null.");
    }

    // TODO: Check prerequsites.
}

bool
PlatformAudioWebRtc::hasSpeaker() const {
    bool isAvailable{false};

    const auto status = audioDeviceModule_->SpeakerMuteIsAvailable(&isAvailable);
    if (status == 0) {
        return isAvailable;
    }

    return false;
}

bool
PlatformAudioWebRtc::setSpeakerOn() {
    if (!hasSpeaker()) {
        return false;
    }

    const auto status = audioDeviceModule_->SetSpeakerMute(false);

    return status == 0;
}

bool
PlatformAudioWebRtc::setSpeakerOff() {
    if (!hasSpeaker()) {
        return false;
    }

    const auto status = audioDeviceModule_->SetSpeakerMute(true);

    return status == 0;
}

bool
PlatformAudioWebRtc::hasMicrophone() const {
    bool isAvailable{false};

    const auto status = audioDeviceModule_->MicrophoneMuteIsAvailable(&isAvailable);
    if (status == 0) {
        return isAvailable;
    }

    return false;
}

bool
PlatformAudioWebRtc::setMicrophoneOn() {
    if (!hasMicrophone()) {
        return false;
    }

    const auto status = audioDeviceModule_->SetMicrophoneMute(false);

    return status == 0;
}

bool
PlatformAudioWebRtc::setMicrophoneOff() {
    if (!hasMicrophone()) {
        return false;
    }

    const auto status = audioDeviceModule_->SetMicrophoneMute(true);

    return status == 0;
}
