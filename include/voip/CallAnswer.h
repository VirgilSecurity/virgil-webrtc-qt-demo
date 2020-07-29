#ifndef VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED
#define VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED

#include <QDateTime>
#include <QUuid>

#include <memory>

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class CallAnswer : public CallSignalingMessage {
    Q_OBJECT

public:
    CallAnswer(QUuid callUUID, QString sdp);

    virtual QJsonObject
    toJson() const override;

    static CallAnswer
    fromJson(const QString &jsonString);

    static CallAnswer
    fromJson(const QJsonObject &json);

    QUuid callUUID() const noexcept;

    QString sdp() const noexcept;

private:
    QUuid m_callUUID;
    QString m_sdp;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED */
