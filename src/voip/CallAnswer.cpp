#include "voip/CallAnswer.h"

using namespace virgil::voip;

CallAnswer::CallAnswer(QUuid callUUID, QString sdp)
    : m_callUUID(std::move(callUUID)), m_sdp(std::move(sdp)) {
}

QJsonObject
CallAnswer::toJson() const {
    return QJsonObject();
}

std::unique_ptr<CallAnswer>
CallAnswer::fromJson(const QString &json_string) {
    return nullptr;
}

std::unique_ptr<CallAnswer>
CallAnswer::fromJson(const QJsonObject &json) {
    return nullptr;
}

QUuid CallAnswer::callUUID() const noexcept {
    return m_callUUID;
}

QString CallAnswer::sdp() const noexcept {
    return m_sdp;
}
