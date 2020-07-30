#include "voip/IceCandidate.h"

#include <QJsonDocument>
#include <QDebug>

#include "voip/CallException.h"

using namespace virgil::voip;

IceCandidate::IceCandidate(QUuid callUUID, int sdpMLineIndex, QString sdpMid, QString sdp)
    : m_callUUID(std::move(callUUID)), m_sdpMLineIndex(sdpMLineIndex), m_sdpMid(std::move(sdpMid)),
      m_sdp(std::move(sdp)) {
}

QJsonObject
IceCandidate::toJson() const {
    auto json = QJsonObject();

    json["callUUID"] = m_callUUID.toString();
    json["sdpMLineIndex"] = m_sdpMLineIndex;
    json["sdpMid"] = m_sdpMid;
    json["sdp"] = m_sdp;

    return json;
}

CallSignalingMessage::Type
IceCandidate::type() const noexcept {
    return CallSignalingMessage::Type::iceCandidate;
}

IceCandidate
IceCandidate::fromJson(const QString &jsonString) {
    QJsonParseError error{};
    auto jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse call offer (invalid JSON): " << error.errorString();
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    if (!jsonDocument.isObject()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    return fromJson(jsonDocument.object());
}

IceCandidate
IceCandidate::fromJson(const QJsonObject &json) {
    //
    //  callUUID
    //
    auto callUUIDValue = json["callUUID"];
    if (!callUUIDValue.isString()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    auto callUUID = QUuid::fromString(callUUIDValue.toString());
    if (callUUID.isNull()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    //
    //  sdpMLineIndex
    //
    auto sdpMLineIndexValue = json["sdpMLineIndex"];
    if (!sdpMLineIndexValue.isDouble()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    auto sdpMLineIndex = sdpMLineIndexValue.toInt();

    //
    //  sdpMid
    //
    auto sdpMidValue = json["sdpMid"];
    if (!sdpMidValue.isString()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    auto sdpMid = sdpMidValue.toString();

    //
    //  sdp
    //
    auto sdpValue = json["sdp"];
    if (!sdpValue.isString()) {
        throw CallException(CallError::FailedToParseIceCandidate);
    }

    auto sdp = sdpValue.toString();

    return IceCandidate(std::move(callUUID), sdpMLineIndex, std::move(sdpMid), std::move(sdp));
}
QUuid
IceCandidate::callUUID() const noexcept {
    return m_callUUID;
}

int
IceCandidate::sdpMLineIndex() const noexcept {
    return m_sdpMLineIndex;
}

QString
IceCandidate::sdpMid() const noexcept {
    return m_sdpMid;
}

QString
IceCandidate::sdp() const noexcept {
    return m_sdp;
}
