#include "voip/CallManager.h"

#include <QDebug>

#include "voip/Call.h"
#include "voip/OutgoingCall.h"
#include "voip/IncomingCall.h"
#include "voip/CallConnectionFactory.h"

using namespace virgil::voip;


CallManager::CallManager(QString myId, std::unique_ptr<PlatformAudio> platformAudio)
    : m_myId(std::move(myId)), m_platformAudio(std::move(platformAudio)) {
    if (!m_platformAudio) {
        m_platformAudio = PlatformAudio::createDefault();
    }
}

const QString &
CallManager::myId() const noexcept {
    return m_myId;
}

std::shared_ptr<OutgoingCall>
CallManager::createOutgoingCall(QString callee) {
    auto callUUID = QUuid::createUuid();

    auto outgoingCall = std::make_shared<OutgoingCall>(std::move(callUUID), m_myId, std::move(callee));

    this->connectCall(outgoingCall);

    m_calls[outgoingCall->uuid()] = outgoingCall;

    return outgoingCall;
}

std::shared_ptr<IncomingCall>
CallManager::createIncomingCall(const CallOffer &callOffer) {
    auto incomingCall = std::make_shared<IncomingCall>(callOffer, m_myId);

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
CallManager::setMicrophoneOn(bool on) {
    for (const auto &callIt : m_calls) {
        auto call = callIt.second;
        call->setMicrophoneOn(on);
    }
}

void
CallManager::setVoiceOn(bool on) {
    for (const auto &callIt : m_calls) {
        auto call = callIt.second;
        call->setVoiceOn(on);
    }
}

bool
CallManager::hasSpeaker() const {
    return m_platformAudio->hasSpeaker();
}

void
CallManager::setSpeakerOn(bool on) {
    m_platformAudio->setSpeakerOn(on);
}

void
CallManager::setHoldOn(bool on) {
    for (const auto &callIt : m_calls) {
        auto call = callIt.second;
        call->setHoldOn(on);
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
