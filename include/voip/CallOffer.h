#ifndef VIRGIL_VOIP_CALL_OFFER_H_INCLUDED
#define VIRGIL_VOIP_CALL_OFFER_H_INCLUDED

#include <QDateTime>
#include <QUuid>

#include <memory>

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class CallOffer : public CallSignalingMessage {
    Q_OBJECT

public:
    CallOffer(QUuid callUUID, QDateTime date, QString caller, QString sdp);

    virtual QJsonObject
    toJson() const override;

    static std::unique_ptr<CallOffer>
    fromJson(const QString &json_string);

    static std::unique_ptr<CallOffer>
    fromJson(const QJsonObject &json);

private:
    QUuid m_callUUID;
    QDateTime m_date;
    QString m_caller;
    QString m_sdp;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_OFFER_H_INCLUDED */
