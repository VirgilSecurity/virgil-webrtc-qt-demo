#ifndef VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED
#define VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED

#include <QDateTime>
#include <QUuid>

#include <memory>

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class IceCandidate : public CallSignalingMessage {
    Q_OBJECT

public:
    IceCandidate(QUuid callUUID, int sdpMLineIndex, QString sdpMid, QString sdp);

    virtual QJsonObject
    toJson() const override;

    virtual CallSignalingMessage::Type
    type() const noexcept override;

    static IceCandidate
    fromJson(const QString &jsonString);

    static IceCandidate
    fromJson(const QJsonObject &json);

    QUuid callUUID() const noexcept;

    int sdpMLineIndex() const noexcept;

    QString sdpMid() const noexcept;

    QString sdp() const noexcept;

private:
    QUuid m_callUUID;
    int m_sdpMLineIndex;
    QString m_sdpMid;
    QString m_sdp;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED */
