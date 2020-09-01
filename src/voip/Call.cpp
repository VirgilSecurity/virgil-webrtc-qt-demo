#include "Call.h"

#include "CallException.h"
#include "IceCandidate.h"
#include "PeerConnectionQueue.h"

// TODO: Qt logging
#include <QDebug>

using namespace virgil::voip;

Call::Call(std::string uuid, std::string myName, std::string otherName)
    : uuid_(std::move(uuid)),
      myName_(std::move(myName)),
      otherName_(std::move(otherName)),
      phase_(CallPhase::initial),
      endReason_(CallEndReason::ended),
      connectionState_(CallConnectionState::none) {
}

void
Call::setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection) {
    CallException::throwIfNull(peerConnection, CallError::NoPeerConnection);

    peerConnection_ = peerConnection;
    connectionState_ = CallConnectionState::none;
}

void
Call::addRemoteIceCandidate(const IceCandidate &iceCandidate) {

    this->doPeerConnectionOp([sdpMid = iceCandidate.sdpMid(),
                              sdpMLineIndex = iceCandidate.sdpMLineIndex(),
                              sdpString = iceCandidate.sdp()](auto peerConnection) {
        webrtc::SdpParseError error = {};

        std::unique_ptr<webrtc::IceCandidateInterface> webrtcIceCandidate{
                webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, sdpString, &error)};

        if (webrtcIceCandidate.get() != nullptr) {
            peerConnection->AddIceCandidate(webrtcIceCandidate.get());
        } else {
            throw CallException(CallError::FailedToParseIceCandidate, error.description);
        }
    });
}

void
Call::update(const CallReceived &callReceived) {
    changePhase(CallPhase::received);
}

void
Call::update(const CallRejected &callRejected) {
    die(CallEndReason::rejected);
}

void
Call::update(const CallEnded &callEnded) {
    die(CallEndReason::ended);
}

void
Call::end(std::optional<CallError> maybeError) {
    auto message = CallEnded(this->uuid());
    sendSignalingMessage(message);

    if (maybeError) {
        die(CallEndReason::failed, maybeError);

    } else if (!isOutgoing() && (phase_ != CallPhase::accepted)) {
        die(CallEndReason::rejected);

    } else {
        die(CallEndReason::ended);
    }
}

void
Call::die(CallError error) {
    die(CallEndReason::failed, error);
}

void
Call::die(CallEndReason endReason, std::optional<CallError> maybeError) {
    this->doPeerConnectionOp([](auto peerConnection) {
        peerConnection->Close();
    });

    peerConnection_ = nullptr;
    connectionState_ = CallConnectionState::none;
    endReason_ = endReason;

    changePhase(CallPhase::ended, maybeError);
}

void
Call::setHoldOn(bool on) {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto transivers = peerConnection->GetTransceivers();

        for (const auto &transiver : transivers) {
            transiver->sender()->track()->set_enabled(!on);
            transiver->receiver()->track()->set_enabled(!on);
        }
    });
}

void
Call::setMicrophoneOn(bool on) {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto senders = peerConnection->GetSenders();

        for (const auto &sender : senders) {
            sender->track()->set_enabled(on);
        }
    });
}

void
Call::setVoiceOn(bool on) {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto receivers = peerConnection->GetReceivers();

        for (const auto &receiver : receivers) {
            receiver->track()->set_enabled(on);
        }
    });
}

void
Call::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) {
    qDebug() << "!!!Call::OnSignalingChange() " << int(newState);
}

void
Call::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface>) {
}

void
Call::OnRenegotiationNeeded() {
    qDebug() << "!!!Call::OnRenegotiationNeeded()";
}

void
Call::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState newState) {
    qDebug() << "!!!Call::OnConnectionChange() " << int(newState);

    switch (newState) {
        case webrtc::PeerConnectionInterface::PeerConnectionState::kNew:
            changeConnectionState(CallConnectionState::initial);
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnecting:
            changeConnectionState(CallConnectionState::connecting);
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnected:
            changeConnectionState(CallConnectionState::connected);
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kDisconnected:
            changeConnectionState(CallConnectionState::disconnected);
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kFailed:
            changeConnectionState(CallConnectionState::failed);
            end(CallError::FailedToConnect);
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kClosed:
            changeConnectionState(CallConnectionState::closed);
            break;

        default:
            break;
    }
}

void
Call::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) {
    qDebug() << "!!!Call::OnIceGatheringChange() " << int(newState);
}

void
Call::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
    std::string sdpString;

    if (!candidate->ToString(&sdpString)) {
        end(CallError::FailedToExportSessionDescription);
        return;
    }

    auto iceCandidate =
            IceCandidate(this->uuid(), candidate->sdp_mline_index(), candidate->sdp_mid(), std::move(sdpString));

    this->sendSignalingMessage(iceCandidate);
}

void
Call::changePhase(CallPhase newPhase, std::optional<CallError> maybeError) noexcept {
    if (newPhase == phase_) {
        return;
    }

    switch (phase_ = newPhase) {
        case CallPhase::initial:
            break;

        case CallPhase::started:
            this->started();
            break;

        case CallPhase::received:
            this->received();
            break;

        case CallPhase::accepted:
            this->accepted();
            break;

        case CallPhase::ended:
            this->ended(maybeError);
            break;
    }
}

void
Call::changeConnectionState(CallConnectionState newState) noexcept {
    if (newState != connectionState_) {
        connectionState_ = newState;
        connectionStateChanged(connectionState_);
    }
}

std::string
Call::uuid() const noexcept {
    return uuid_;
}

std::string
Call::myName() const noexcept {
    return myName_;
}

std::string
Call::otherName() const noexcept {
    return otherName_;
}

std::optional<std::time_t>
Call::connectedAt() const noexcept {
    return connectedAt_;
}

CallPhase
Call::phase() const noexcept {
    return phase_;
}

CallEndReason
Call::endReason() const noexcept {
    return endReason_;
}

void
Call::doPeerConnectionOp(std::function<void(rtc::scoped_refptr<webrtc::PeerConnectionInterface>)> op) {
    if (peerConnection_) {
        PeerConnectionQueue::sharedInstance().dispatch([peerConnection = peerConnection_, op] {
            op(peerConnection);
        });
    }
}
