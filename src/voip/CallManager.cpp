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

std::shared_ptr<Call>
CallManager::findCall(const QUuid &uuid) {
    auto call = m_calls.find(uuid);
    if (call != m_calls.end()) {
        return call->second;
    }

    return nullptr;
}


std::shared_ptr<IncomingCall>
CallManager::findIncomingCall(const QUuid &uuid) {
    auto call = m_calls.find(uuid);
    if (call != m_calls.end() && !call->second->isOutgoing()) {
        return std::static_pointer_cast<IncomingCall>(call->second);
    }

    return nullptr;
}

std::shared_ptr<OutgoingCall>
CallManager::findOutgoingCall(const QUuid &uuid) {
    auto call = m_calls.find(uuid);
    if (call != m_calls.end() && call->second->isOutgoing()) {
        return std::static_pointer_cast<OutgoingCall>(call->second);
    }

    return nullptr;
}


void
CallManager::processCallAnswer(const CallAnswer &callAnswer) {
    auto call = findOutgoingCall(callAnswer.callUUID());
    if (call) {
        call->accept(
                callAnswer,
                [] {
                },
                [this, call](CallError error) {
                    callFailed(*call, error);
                });
    }
}


void
CallManager::processIceCandidate(const IceCandidate &iceCandidate) {
    auto call = findCall(iceCandidate.callUUID());
    if (call) {
        call->addRemoteIceCandidate(iceCandidate);
    }
}


void
CallManager::connectCall(std::shared_ptr<Call> call) {

    call->phaseChanged.connect([this, call](CallPhase newPhase) {
        this->callPhaseChanged(*call, newPhase);
    });

    call->connectionStateChanged.connect([this, call](CallConnectionState newConnectionState) {
        this->callConnectionStateChanged(*call, newConnectionState);
    });

    call->createdSignalingMessage.connect([this, call](const CallSignalingMessage &signalingMessage) {
        this->createdMessageToSent(*call, signalingMessage);
    });
}
