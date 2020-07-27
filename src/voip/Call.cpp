#include "voip/Call.h"

#include "voip/CallException.h"

#include <QDebug>

using namespace virgil::voip;

Call::Call(QObject *parent, QString uuid, QString myName, QString otherName)
    : QObject(parent), m_uuid(std::move(uuid)), m_myName(std::move(myName)), m_otherName(std::move(otherName)) {
}

void
Call::setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection) {
    // TODO: Add preconditions check.
    m_peerConnection = peerConnection;
}

void
Call::end() noexcept {
    m_peerConnection->Close();
    m_peerConnection = nullptr;
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
Call::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) {
    qInfo() << "Call::OnSignalingChange()";
}

void
Call::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {
    qInfo() << "Call::OnDataChannel()";
}

void
Call::OnRenegotiationNeeded() {
    qInfo() << "Call::OnRenegotiationNeeded()";
}

void
Call::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) {
    qInfo() << "Call::OnConnectionChange()";
}

void
Call::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) {
    qInfo() << "Call::OnIceGatheringChange()";
}

void
Call::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
    qInfo() << "Call::OnIceCandidate()";
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

QUuid Call::uuid() const noexcept {
    return m_uuid;
}

QString Call::myName() const noexcept {
    return m_myName;
}

QString Call::otherName() const noexcept {
    return m_otherName;
}

std::optional<QDateTime> Call::createdAt() const noexcept {
    return m_createdAt;
}
