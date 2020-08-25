#include "PlatformAudio.h"

#include "PlatformAudioWebRtc.h"
#include "WebRtcResource.h"

#if defined(WEBRTC_ANDROID)
#include "android/PlatformAudioAndroid.h"
#endif


using namespace virgil::voip;


std::unique_ptr<PlatformAudio>
PlatformAudio::createDefault() {
#if defined(WEBRTC_ANDROID)
    return std::make_unique<PlatformAudioAndroid>();
#else
    auto audioDeviceModule = WebRtcResource::sharedInstance().audioDeviceModule();
    return std::make_unique<PlatformAudioWebRtc>(std::move(audioDeviceModule));
#endif
}
