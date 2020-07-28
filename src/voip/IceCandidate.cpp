#include "voip/IceCandidate.h"

using namespace virgil::voip;

IceCandidate::IceCandidate(QUuid callUUID, int sdpMLineIndex, QString sdpMid, QString sdp)
    : m_callUUID(std::move(callUUID)), m_sdpMLineIndex(sdpMLineIndex), m_sdpMid(std::move(sdpMid)), m_sdp(std::move(sdp)) {
}

QJsonObject
IceCandidate::toJson() const {
    return QJsonObject();
}

std::unique_ptr<IceCandidate>
IceCandidate::fromJson(const QString &json_string) {
    return nullptr;
}

std::unique_ptr<IceCandidate>
IceCandidate::fromJson(const QJsonObject &json) {
    return nullptr;
}

QUuid IceCandidate::callUUID() const noexcept {
    return m_callUUID;
}

int IceCandidate::sdpMLineIndex() const noexcept {
    return m_sdpMLineIndex;
}

QString IceCandidate::sdpMid() const noexcept {
    return m_sdpMid;
}

QString IceCandidate::sdp() const noexcept {
    return m_sdp;
}
