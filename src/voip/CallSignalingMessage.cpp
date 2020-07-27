#include "voip/CallSignalingMessage.h"

#include <QJsonDocument>

using namespace virgil::voip;

QString
CallSignalingMessage::toJsonString() const {
    auto json = this->toJson();

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact));
}
