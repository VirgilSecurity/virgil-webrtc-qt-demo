#include "voip/Call.h"

#include "voip/CallException.h"
#include "voip/IceCandidate.h"

#include <QDebug>

using namespace virgil::voip;

Call::Call(QObject *parent, QUuid uuid, QString myName, QString otherName)
    : QObject(parent), m_uuid(std::move(uuid)), m_myName(std::move(myName)), m_otherName(std::move(otherName)),
      m_phase(CallPhase::initial), m_connectionState(CallConnectionState::none) {
}

void
Call::setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection) {
    CallException::throwIfNull(peerConnection, CallError::NoPeerConnection);

    m_peerConnection = peerConnection;
}

void
Call::end() noexcept {
    m_peerConnection->Close();
    m_peerConnection = nullptr;
    m_phase = CallPhase::ended;
    m_connectionState = CallConnectionState::none;
}

void
Call::hold(bool onHold) noexcept {
    if (!m_peerConnection) {
        return;
    }

    auto transivers = m_peerConnection->GetTransceivers();

    for (const auto &transiver : transivers) {
        transiver->sender()->track()->set_enabled(!onHold);
    }
}

void
Call::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) {
    qDebug() << "Call::OnSignalingChange()" << (int)newState;
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

    Q_EMIT createdSignalingMessage(iceCandidate);
}

rtc::scoped_refptr<webrtc::PeerConnectionInterface>
Call::peerConnection() {

    CallException::throwIfNull(m_peerConnection, CallError::NoPeerConnection);

    return m_peerConnection;
}

const rtc::scoped_refptr<webrtc::PeerConnectionInterface>
Call::peerConnection() const {

    CallException::throwIfNull(m_peerConnection, CallError::NoPeerConnection);

    return m_peerConnection;
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
        Q_EMIT connectionStateChanged(m_connectionState);
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
