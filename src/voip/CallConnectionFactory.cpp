#include "voip/CallConnectionFactory.h"

#include "voip/CallException.h"

using namespace virgil::voip;

CallConnectionFactory::CallConnectionFactory() {
    m_peerConnectionFactory = webrtc::CreateModularPeerConnectionFactory(webrtc::PeerConnectionFactoryDependencies());
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

    publicStunIceServers.urls = {"stun:stun.l.google.com:19302", "stun:stun1.l.google.com:19302",
            "stun:stun2.l.google.com:19302", "stun:stun3.l.google.com:19302", "stun:stun4.l.google.com:19302"};


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
