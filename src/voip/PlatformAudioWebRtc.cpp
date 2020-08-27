#include "PlatformAudioWebRtc.h"

#include "WebRtcResource.h"

#include <stdexcept>

using namespace virgil::voip;

PlatformAudioWebRtc::PlatformAudioWebRtc(rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule)
    : audioDeviceModule_(std::move(audioDeviceModule)) {

    if (!audioDeviceModule_) {
        throw std::logic_error("PlatformAudioWebRtc: audioDeviceModule is null.");
    }
}

bool
PlatformAudioWebRtc::hasSpeaker() const {
    return false;
}

void
PlatformAudioWebRtc::setSpeakerOn(bool on) {
    (void)on;
    throw std::logic_error("PlatformAudioWebRtc::setSpeakerOn() is not supported.");
}

std::unique_ptr<PlatformAudio>
PlatformAudio::createDefault() {
    auto audioDeviceModule = WebRtcResource::sharedInstance().audioDeviceModule();
    return std::make_unique<PlatformAudioWebRtc>(std::move(audioDeviceModule));
}
