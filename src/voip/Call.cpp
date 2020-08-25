#include "voip/Call.h"

#include "voip/CallException.h"
#include "voip/IceCandidate.h"
#include "voip/PeerConnectionQueue.h"

#include <QDebug>

using namespace virgil::voip;

Call::Call(QUuid uuid, QString myName, QString otherName)
    : m_uuid(std::move(uuid)),
      m_myName(std::move(myName)),
      m_otherName(std::move(otherName)),
      m_phase(CallPhase::initial),
      m_connectionState(CallConnectionState::none) {
}

void
Call::setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection) {
    CallException::throwIfNull(peerConnection, CallError::NoPeerConnection);

    m_peerConnection = peerConnection;
}

void
Call::addRemoteIceCandidate(const IceCandidate &iceCandidate) {

    auto sdpMid = iceCandidate.sdpMid().toStdString();
    auto sdpMLineIndex = iceCandidate.sdpMLineIndex();
    auto sdpString = iceCandidate.sdp().toStdString();

    this->doPeerConnectionOp([sdpMid, sdpMLineIndex, sdpString](auto peerConnection) {
        webrtc::SdpParseError error = {};

        std::unique_ptr<webrtc::IceCandidateInterface> webrtcIceCandidate{
                webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, sdpString, &error)};

        if (webrtcIceCandidate.get() != nullptr) {
            peerConnection->AddIceCandidate(webrtcIceCandidate.get());
        } else {
            qWarning() << "Failed to parse ice candidate: " << QString::fromStdString(error.description);
        }
    });
}

void
Call::end() noexcept {
    this->doPeerConnectionOp([](auto peerConnection) {
        peerConnection->Close();
    });

    m_peerConnection = nullptr;
    m_phase = CallPhase::ended;
    m_connectionState = CallConnectionState::none;
}

void
Call::setHoldOn(bool on) noexcept {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto transivers = peerConnection->GetTransceivers();

        for (const auto &transiver : transivers) {
            transiver->sender()->track()->set_enabled(!on);
            transiver->receiver()->track()->set_enabled(!on);
        }
    });
}

void
Call::setMicrophoneOn(bool on) noexcept {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto senders = peerConnection->GetSenders();

        for (const auto &sender : senders) {
            sender->track()->set_enabled(on);
        }
    });
}

void
Call::setVoiceOn(bool on) noexcept {
    this->doPeerConnectionOp([on](auto peerConnection) {
        auto receivers = peerConnection->GetReceivers();

        for (const auto &receiver : receivers) {
            receiver->track()->set_enabled(on);
        }
    });
}

void
Call::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) {
    qDebug() << "Call::OnSignalingChange() " << m_uuid.toString() << " " << (int)newState;
}

void
Call::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface>) {
    qCritical() << "Call::OnDataChannel() - not supported yet";
}

void
Call::OnRenegotiationNeeded() {
    qDebug() << "Call::OnRenegotiationNeeded()";
}

void
Call::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState newState) {
    qDebug() << "Call::OnConnectionChange()" << (int)newState;

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
            break;

        case webrtc::PeerConnectionInterface::PeerConnectionState::kClosed:
            changeConnectionState(CallConnectionState::closed);
            break;

        default:
            qWarning() << "Undefined PeerConnectionState: " << (int)newState;
            break;
    }
}

void
Call::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) {
    qDebug() << "Call::OnIceGatheringChange()" << (int)newState;
}

void
Call::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
    qDebug() << "Call::OnIceCandidate()";

    std::string sdpString;

    if (!candidate->ToString(&sdpString)) {
        qCritical() << "Failed to export session description of an ice candidate";
        return;
    }

    auto sdpMid = QString::fromStdString(candidate->sdp_mid());
    auto sdp = QString::fromStdString(sdpString);


    auto iceCandidate = IceCandidate(this->uuid(), candidate->sdp_mline_index(), std::move(sdpMid), std::move(sdp));

    this->createdSignalingMessage(iceCandidate);
}

void
Call::changePhase(CallPhase newPhase) noexcept {
    if (newPhase != m_phase) {
        m_phase = newPhase;
        Q_EMIT phaseChanged(m_phase);
    }
}

void
Call::changeConnectionState(CallConnectionState newState) noexcept {
    if (newState != m_connectionState) {
        m_connectionState = newState;
        connectionStateChanged(m_connectionState);
    }
}

QUuid
Call::uuid() const noexcept {
    return m_uuid;
}

QString
Call::myName() const noexcept {
    return m_myName;
}

QString
Call::otherName() const noexcept {
    return m_otherName;
}

std::optional<QDateTime>
Call::connectedAt() const noexcept {
    return m_connectedAt;
}

void
Call::doPeerConnectionOp(std::function<void(rtc::scoped_refptr<webrtc::PeerConnectionInterface>)> op) {
    PeerConnectionQueue::sharedInstance().dispatch([peerConnection = m_peerConnection, op] {
        op(peerConnection);
    });
}
