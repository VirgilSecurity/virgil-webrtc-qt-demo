#include "CallConnectionFactory.h"

#include "WebRtcResource.h"
#include "CallException.h"

#include <webrtc/api/audio_codecs/builtin_audio_decoder_factory.h>
#include <webrtc/api/audio_codecs/builtin_audio_encoder_factory.h>
#include <webrtc/api/rtc_event_log/rtc_event_log_factory.h>
#include <webrtc/api/task_queue/default_task_queue_factory.h>
#include <webrtc/media/engine/webrtc_media_engine.h>

#include <array>

using namespace virgil::voip;


CallConnectionFactory &
CallConnectionFactory::sharedInstance() {
    static CallConnectionFactory instance;
    return instance;
}

CallConnectionFactory::CallConnectionFactory() {
    auto &sharedResource = WebRtcResource::sharedInstance();

    webrtc::PeerConnectionFactoryDependencies dependencies;
    dependencies.network_thread = sharedResource.networkThread();
    dependencies.worker_thread = sharedResource.workerThread();
    dependencies.signaling_thread = sharedResource.signalingThread();
    dependencies.task_queue_factory = webrtc::CreateDefaultTaskQueueFactory();

    cricket::MediaEngineDependencies mediaDependencies;
    mediaDependencies.adm = sharedResource.audioDeviceModule();
    mediaDependencies.task_queue_factory = dependencies.task_queue_factory.get();
    mediaDependencies.audio_encoder_factory = webrtc::CreateBuiltinAudioEncoderFactory();
    mediaDependencies.audio_decoder_factory = webrtc::CreateBuiltinAudioDecoderFactory();
    mediaDependencies.audio_processing = webrtc::AudioProcessingBuilder().Create();

    dependencies.media_engine = cricket::CreateMediaEngine(std::move(mediaDependencies));
    dependencies.call_factory = webrtc::CreateCallFactory();
    dependencies.event_log_factory =
            std::make_unique<webrtc::RtcEventLogFactory>(dependencies.task_queue_factory.get());

    factory_ = webrtc::CreateModularPeerConnectionFactory(std::move(dependencies));
}

void
CallConnectionFactory::setupPeerConnection(Call &call) const {
    rtc::CritScope lock(&factoryMutex_);

    //
    //  Basic configuration.
    //
    static std::initializer_list<std::string> urls{"stun:stun.l.google.com:19302",
                                                   "stun:stun1.l.google.com:19302",
                                                   "stun:stun2.l.google.com:19302",
                                                   "stun:stun3.l.google.com:19302",
                                                   "stun:stun4.l.google.com:19302"};

    auto publicStunIceServers = webrtc::PeerConnectionInterface::IceServer();
    publicStunIceServers.urls = urls;

    auto rtcConfig = webrtc::PeerConnectionInterface::RTCConfiguration();
    rtcConfig.servers.emplace_back(std::move(publicStunIceServers));
    rtcConfig.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    rtcConfig.continual_gathering_policy = webrtc::PeerConnectionInterface::GATHER_CONTINUALLY;

    //
    //  Add media streams.
    //
    auto peerConnection = factory_->CreatePeerConnection(rtcConfig, webrtc::PeerConnectionDependencies(&call));
    CallException::throwIfNull(peerConnection, CallError::FailedToConfigurePeerConnection);

    auto audioSource = factory_->CreateAudioSource(cricket::AudioOptions());
    CallException::throwIfNull(audioSource, CallError::FailedToConfigurePeerConnection);

    auto audioTrack = factory_->CreateAudioTrack("audio_track_0", audioSource);
    CallException::throwIfNull(audioTrack, CallError::FailedToConfigurePeerConnection);

    peerConnection->AddTrack(audioTrack, {"audio_stream"});

    call.setupPeerConnection(peerConnection);
}
