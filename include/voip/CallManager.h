#ifndef VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED
#define VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED

#include <QObject>
#include <QString>
#include <QUuid>

#include <unordered_map>
#include <memory>

#include "Call.h"
#include "IncomingCall.h"
#include "OutgoingCall.h"

#include "utils/quuid_unordered_map_spec.h"

namespace virgil {
namespace voip {


class CallManager : public QObject {
    Q_OBJECT

public:
    CallManager();

    static CallManager&
    sharedInstance();

    std::shared_ptr<OutgoingCall>
    createOutgoingCall(QString callee);

    std::shared_ptr<IncomingCall>
    createIncomingCall(const CallOffer& callOffer);

Q_SIGNALS:
    void
    callPhaseChanged(std::shared_ptr<Call> call, CallPhase newPhase);

    void
    callConnectionStateChanged(std::shared_ptr<Call> call, CallConnectionState newConnectionState);

    void
    createdMessageToSent(const CallSignalingMessage& message);

private:
    void
    connectCall(std::shared_ptr<Call> call);

private:
    std::unordered_map<QUuid, std::shared_ptr<Call>> m_calls;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED */
