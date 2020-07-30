#include "voip/CallManager.h"

#include <QDebug>

#include "voip/Call.h"
#include "voip/OutgoingCall.h"
#include "voip/IncomingCall.h"

using namespace virgil::voip;


CallManager::CallManager() {
}

CallManager &
CallManager::sharedInstance() {
    static CallManager callManager{};
    return callManager;
}

std::shared_ptr<OutgoingCall>
CallManager::createOutgoingCall(QString callee) {
    auto callUUID = QUuid::createUuid();

    auto outgoingCall = std::make_shared<OutgoingCall>(std::move(callUUID), "test_user", std::move(callee));

    this->connectCall(outgoingCall);

    m_calls[outgoingCall->uuid()] = outgoingCall;

    return outgoingCall;
}

std::shared_ptr<IncomingCall>
CallManager::createIncomingCall(const CallOffer &callOffer) {
    auto incomingCall = std::make_shared<IncomingCall>(callOffer, "test_user");

    this->connectCall(incomingCall);

    m_calls[incomingCall->uuid()] = incomingCall;

    return incomingCall;
}

void
CallManager::connectCall(std::shared_ptr<Call> call) {

    qRegisterMetaType<CallPhase>("CallPhase");
    qRegisterMetaType<CallConnectionState>("CallConnectionState");


    QObject::connect(call.get(), &Call::phaseChanged, this, [this, call](CallPhase newPhase) {
        Q_EMIT this->callPhaseChanged(call, newPhase);
    });

    QObject::connect(call.get(),
            &Call::connectionStateChanged,
            this,
            [this, call](CallConnectionState newConnectionState) {
                Q_EMIT this->callConnectionStateChanged(call, newConnectionState);
            });

    QObject::connect(call.get(),
            &Call::createdSignalingMessage,
            this,
            [this, call](std::shared_ptr<CallSignalingMessage> signalingMessage) {
                Q_EMIT this->createdMessageToSent(signalingMessage);
            });
}
