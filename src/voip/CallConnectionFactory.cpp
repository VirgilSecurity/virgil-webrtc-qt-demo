#include "voip/CallConnectionFactory.h"

#include <webrtc/rtc_base/ssl_adapter.h>
#include <webrtc/api/task_queue/default_task_queue_factory.h>
#include <webrtc/api/rtc_event_log/rtc_event_log_factory.h>
#include <webrtc/media/engine/webrtc_media_engine.h>
#include <webrtc/api/audio_codecs/builtin_audio_decoder_factory.h>
#include <webrtc/api/audio_codecs/builtin_audio_encoder_factory.h>

#include "voip/CallException.h"

using namespace virgil::voip;

CallConnectionFactory::CallConnectionFactory() {
    m_thread_checker.Detach();

    //
    // Setup WebRTC global environment.
    //
    rtc::LogMessage::SetLogToStderr(true);
    rtc::LogMessage::LogTimestamps();
    rtc::LogMessage::LogThreads();
    rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_VERBOSE);

    const bool isInitializedSSL = rtc::InitializeSSL();
    CallException::throwIfFalse(isInitializedSSL,
            CallError::FailedToConfigureWebRTC,
            "Failed to initialize WebRTC SSL");

    //
    // Configure WebRTC.
    //
    m_network_thread = rtc::Thread::CreateWithSocketServer();
    m_network_thread->SetName("network_thread", nullptr);
    CallException::throwIfFalse(m_network_thread->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC network thread");

    m_worker_thread = rtc::Thread::Create();
    m_worker_thread->SetName("worker_thread", nullptr);
    CallException::throwIfFalse(m_worker_thread->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC worker thread");

    m_signaling_thread = rtc::Thread::Create();
    m_signaling_thread->SetName("signaling_thread", nullptr);
    CallException::throwIfFalse(m_signaling_thread->Start(),
            CallError::FailedToConfigureWebRTC,
            "Failed to start WebRTC signaling thread");

    webrtc::PeerConnectionFactoryDependencies dependencies;
    dependencies.network_thread = m_network_thread.get();
    dependencies.worker_thread = m_worker_thread.get();
    dependencies.signaling_thread = m_signaling_thread.get();
    dependencies.task_queue_factory = webrtc::CreateDefaultTaskQueueFactory();

    cricket::MediaEngineDependencies mediaDependencies;
    mediaDependencies.task_queue_factory = dependencies.task_queue_factory.get();
    mediaDependencies.audio_encoder_factory = webrtc::CreateBuiltinAudioEncoderFactory();
    mediaDependencies.audio_decoder_factory = webrtc::CreateBuiltinAudioDecoderFactory();
    mediaDependencies.audio_processing = webrtc::AudioProcessingBuilder().Create();

    dependencies.media_engine = cricket::CreateMediaEngine(std::move(mediaDependencies));
    dependencies.call_factory = webrtc::CreateCallFactory();
    dependencies.event_log_factory =
            std::make_unique<webrtc::RtcEventLogFactory>(dependencies.task_queue_factory.get());

    m_peerConnectionFactory = webrtc::CreateModularPeerConnectionFactory(std::move(dependencies));
}

CallConnectionFactory::~CallConnectionFactory() {
    rtc::CleanupSSL();
}

CallConnectionFactory &
CallConnectionFactory::sharedInstance() {
    static CallConnectionFactory instance;
    return instance;
}

void
CallConnectionFactory::setupPeerConnection(Call &call) const {
    //
    //  Basic configuration.
    //
    auto publicStunIceServers = webrtc::PeerConnectionInterface::IceServer();

    publicStunIceServers.urls = {"stun:stun.l.google.com:19302",
            "stun:stun1.l.google.com:19302",
            "stun:stun2.l.google.com:19302",
            "stun:stun3.l.google.com:19302",
            "stun:stun4.l.google.com:19302"};


    auto rtcConfig = webrtc::PeerConnectionInterface::RTCConfiguration();

    rtcConfig.servers.emplace_back(std::move(publicStunIceServers));
    rtcConfig.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    rtcConfig.continual_gathering_policy = webrtc::PeerConnectionInterface::GATHER_CONTINUALLY;

    //
    //  Define dependencies.
    //
    auto peerConnection =
            m_peerConnectionFactory->CreatePeerConnection(rtcConfig, webrtc::PeerConnectionDependencies(&call));
    CallException::throwIfNull(peerConnection, CallError::FailedToConfigurePeerConnection);

    auto audioSource = m_peerConnectionFactory->CreateAudioSource(cricket::AudioOptions());
    CallException::throwIfNull(audioSource, CallError::FailedToConfigurePeerConnection);

    auto audioTrack = m_peerConnectionFactory->CreateAudioTrack("audio0", audioSource);
    CallException::throwIfNull(audioTrack, CallError::FailedToConfigurePeerConnection);

    peerConnection->AddTrack(audioTrack, {"audio_stream"});

    call.setupPeerConnection(peerConnection);
}
