#include "WebRtcResource.h"

#include "CallException.h"

#include <webrtc/rtc_base/logging.h>
#include <webrtc/rtc_base/ssl_adapter.h>
#include <webrtc/api/task_queue/default_task_queue_factory.h>

#include <stdexcept>


using namespace virgil::voip;


WebRtcResource &
WebRtcResource::sharedInstance() {
    static WebRtcResource instance{};
    return instance;
}

WebRtcResource::WebRtcResource() {
    //
    // Setup WebRTC global environment.
    //
    rtc::LogMessage::SetLogToStderr(true);
    rtc::LogMessage::LogTimestamps();
    rtc::LogMessage::LogThreads();
    // TODO: Remove from release.
    rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_VERBOSE);

    const bool isInitializedSSL = rtc::InitializeSSL();
    CallException::throwIfFalse(
            isInitializedSSL,
            CallError::FailedToConfigureWebRTC,
            "Failed to initialize WebRTC SSL");

    //
    // Configure WebRTC.
    //
    networkThread_ = rtc::Thread::CreateWithSocketServer();
    networkThread_->SetName("network_thread", nullptr);
    CallException::throwIfFalse(
            networkThread_->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC network thread");

    workerThread_ = rtc::Thread::Create();
    workerThread_->SetName("worker_thread", nullptr);
    CallException::throwIfFalse(
            workerThread_->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC worker thread");

    signalingThread_ = rtc::Thread::Create();
    signalingThread_->SetName("signaling_thread", nullptr);
    CallException::throwIfFalse(
            signalingThread_->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC signaling thread");


    audioDeviceModule_ = workerThread_->Invoke<rtc::scoped_refptr<webrtc::AudioDeviceModule> >(RTC_FROM_HERE, [] {
        auto taskQueueFactory = webrtc::CreateDefaultTaskQueueFactory();
        return webrtc::AudioDeviceModule::Create(
                webrtc::AudioDeviceModule::kPlatformDefaultAudio,
                taskQueueFactory.get());
    });
}

WebRtcResource::~WebRtcResource() noexcept {
    try {
        // Should not throw, but just in case.
        rtc::CleanupSSL();
    } catch (const std::exception &) {
        // TODO: Log this.
    }
}

rtc::Thread *
WebRtcResource::networkThread() noexcept {
    return networkThread_.get();
}

rtc::Thread *
WebRtcResource::workerThread() noexcept {
    return workerThread_.get();
}

rtc::Thread *
WebRtcResource::signalingThread() noexcept {
    return signalingThread_.get();
}

rtc::scoped_refptr<webrtc::AudioDeviceModule>
WebRtcResource::audioDeviceModule() noexcept {
    return audioDeviceModule_;
}
