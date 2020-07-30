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

class CallOffer;
class CallAnswer;
class IceCandidate;

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

    std::shared_ptr<Call>
    findCall(const QUuid& uuid);

    std::shared_ptr<IncomingCall>
    findIncomingCall(const QUuid& uuid);

    std::shared_ptr<OutgoingCall>
    findOutgoingCall(const QUuid& uuid);

    void
    processCallAnswer(const CallAnswer& callAnswer);

    void
    processIceCandidate(const IceCandidate& iceCandidate);


Q_SIGNALS:
    void
    callPhaseChanged(std::shared_ptr<Call> call, CallPhase newPhase);

    void
    callConnectionStateChanged(std::shared_ptr<Call> call, CallConnectionState newConnectionState);

    void
    callFailed(std::shared_ptr<Call> call, CallError error);

    void
    createdMessageToSent(CallSignalingMessage* message);

private:
    void
    connectCall(std::shared_ptr<Call> call);


private:
    std::unordered_map<QUuid, std::shared_ptr<Call>> m_calls;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED */
