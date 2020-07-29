#include "voip/CallOffer.h"

#include <QJsonDocument>
#include <QDebug>

#include "voip/CallException.h"

using namespace virgil::voip;

CallOffer::CallOffer(QUuid callUUID, QDateTime date, QString caller, QString sdp)
    : m_callUUID(std::move(callUUID)), m_date(std::move(date)), m_caller(std::move(caller)), m_sdp(std::move(sdp)) {
}

QJsonObject
CallOffer::toJson() const {
    auto json = QJsonObject();

    json["callUUID"] = m_callUUID.toString();
    json["date"] = m_date.toString();
    json["caller"] = m_caller;
    json["sdp"] = m_sdp;

    return json;
}

CallOffer
CallOffer::fromJson(const QString &jsonString) {
    QJsonParseError error{};
    auto jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse call offer (invalid JSON): " << error.errorString();
        throw CallException(CallError::FailedToParseCallOffer);
    }

    if (!jsonDocument.isObject()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    return fromJson(jsonDocument.object());
}

CallOffer
CallOffer::fromJson(const QJsonObject &json) {
    //
    //  callUUID
    //
    auto callUUIDValue = json["callUUID"];
    if (!callUUIDValue.isString()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    auto callUUID = QUuid::fromString(callUUIDValue.toString());
    if (callUUID.isNull()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    //
    //  date
    //
    auto dateValue = json["date"];
    if (!dateValue.isString()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    auto date = QDateTime::fromString(dateValue.toString());
    if (!date.isValid()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    //
    //  caller
    //
    auto callerValue = json["caller"];
    if (!callerValue.isString()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    auto caller = callerValue.toString();

    //
    //  sdp
    //
    auto sdpValue = json["sdp"];
    if (!sdpValue.isString()) {
        throw CallException(CallError::FailedToParseCallOffer);
    }

    auto sdp = sdpValue.toString();

    return CallOffer(std::move(callUUID), std::move(date), std::move(caller), std::move(sdp));
}

QUuid
CallOffer::callUUID() const noexcept {
    return m_callUUID;
}

QDateTime
CallOffer::date() const noexcept {
    return m_date;
}

QString
CallOffer::caller() const noexcept {
    return m_caller;
}

QString
CallOffer::sdp() const noexcept {
    return m_sdp;
}
