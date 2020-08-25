#ifndef VIRGIL_VOIP_WEBRTC_RESOURCE_H_INCLUDED
#define VIRGIL_VOIP_WEBRTC_RESOURCE_H_INCLUDED

#include <webrtc/rtc_base/thread.h>
#include <webrtc/modules/audio_device/include/audio_device.h>

#include <memory>

namespace virgil {
namespace voip {

class WebRtcResource {
public:
    //
    //  Return WebRTC share resources.
    //  Note, can throw exception first time if WebRTC initialization fails.
    //
    static WebRtcResource &
    sharedInstance();

    rtc::Thread *
    networkThread() noexcept;

    rtc::Thread *
    workerThread() noexcept;

    rtc::Thread *
    signalingThread() noexcept;

    rtc::scoped_refptr<webrtc::AudioDeviceModule>
    audioDeviceModule() noexcept;

private:
    //
    //  Initilize WebRTC environment and shared resources.
    //  Note, can throw exception.
    //
    WebRtcResource();

    //
    //  Cleanup WebRTC environment.
    //
    ~WebRtcResource() noexcept;

private:
    std::unique_ptr<rtc::Thread> networkThread_;
    std::unique_ptr<rtc::Thread> workerThread_;
    std::unique_ptr<rtc::Thread> signalingThread_;

    rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule_;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_WEBRTC_RESOURCE_H_INCLUDED */
