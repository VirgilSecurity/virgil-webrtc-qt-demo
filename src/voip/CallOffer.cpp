#include "voip/CallOffer.h"

using namespace virgil::voip;

CallOffer::CallOffer(QUuid callUUID, QDateTime date, QString caller, QString sdp)
    : m_callUUID(std::move(callUUID)), m_date(std::move(date)), m_caller(std::move(caller)), m_sdp(std::move(sdp)) {
}

QJsonObject
CallOffer::toJson() const {
    return QJsonObject();
}

std::unique_ptr<CallOffer>
CallOffer::fromJson(const QString &json_string) {
    return nullptr;
}

std::unique_ptr<CallOffer>
CallOffer::fromJson(const QJsonObject &json) {
    return nullptr;
}
