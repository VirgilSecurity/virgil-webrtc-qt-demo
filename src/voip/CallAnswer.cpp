#include "voip/CallAnswer.h"

#include <QJsonDocument>
#include <QDebug>

#include "voip/CallException.h"

using namespace virgil::voip;

CallAnswer::CallAnswer(QUuid callUUID, QString sdp)
    : m_callUUID(std::move(callUUID)), m_sdp(std::move(sdp)) {
}

QJsonObject
CallAnswer::toJson() const {
    auto json = QJsonObject();

    json["callUUID"] = m_callUUID.toString();
    json["sdp"] = m_sdp;

    return json;
}

CallAnswer
CallAnswer::fromJson(const QString &jsonString) {
    QJsonParseError error{};
    auto jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse call answer (invalid JSON): " << error.errorString();
        throw CallException(CallError::FailedToParseCallAnswer);
    }

    if (!jsonDocument.isObject()) {
        throw CallException(CallError::FailedToParseCallAnswer);
    }

    return fromJson(jsonDocument.object());
}

CallAnswer
CallAnswer::fromJson(const QJsonObject &json) {
    //
    //  callUUID
    //
    auto callUUIDValue = json["callUUID"];
    if (!callUUIDValue.isString()) {
        throw CallException(CallError::FailedToParseCallAnswer);
    }

    auto callUUID = QUuid::fromString(callUUIDValue.toString());
    if (callUUID.isNull()) {
        throw CallException(CallError::FailedToParseCallAnswer);
    }

    //
    //  sdp
    //
    auto sdpValue = json["sdp"];
    if (!sdpValue.isString()) {
        throw CallException(CallError::FailedToParseCallAnswer);
    }

    auto sdp = sdpValue.toString();

    return CallAnswer(std::move(callUUID), std::move(sdp));
}

QUuid CallAnswer::callUUID() const noexcept {
    return m_callUUID;
}

QString CallAnswer::sdp() const noexcept {
    return m_sdp;
}
