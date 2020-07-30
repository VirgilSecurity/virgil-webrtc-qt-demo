#include "EchoCall.h"

#include <QDebug>

namespace voip = virgil::voip;

EchoCall::EchoCall(QObject *parent)
    : QObject(parent),
      m_caller(std::make_unique<voip::CallManager>()),
      m_callee(std::make_unique<voip::CallManager>()) {

    //
    //  Connect Caller.
    //
    QObject::connect(m_caller.get(), &voip::CallManager::callPhaseChanged, this, &EchoCall::outgoingCallPhaseChanged);

    QObject::connect(m_caller.get(),
            &voip::CallManager::callConnectionStateChanged,
            this,
            &EchoCall::outgoingCallConnectionStateChanged);

    QObject::connect(m_caller.get(),
            &voip::CallManager::createdMessageToSent,
            this,
            &EchoCall::outgoingCreatedMessageToSent);

    QObject::connect(m_caller.get(), &voip::CallManager::callFailed, this, &EchoCall::outgoingCallFailed);

    //
    //  Connect Callee.
    //
    QObject::connect(m_callee.get(), &voip::CallManager::callPhaseChanged, this, &EchoCall::incomingCallPhaseChanged);

    QObject::connect(m_callee.get(),
            &voip::CallManager::callConnectionStateChanged,
            this,
            &EchoCall::incomingCallConnectionStateChanged);

    QObject::connect(m_callee.get(),
            &voip::CallManager::createdMessageToSent,
            this,
            &EchoCall::incomingCreatedMessageToSent);

    QObject::connect(m_callee.get(), &voip::CallManager::callFailed, this, &EchoCall::incomingCallFailed);
}


void
EchoCall::call() {
    auto outgoingCall = m_caller->createOutgoingCall("its_me");
    outgoingCall->start(
            [this]() {
                logMessage("Call started...");
            },
            [this](voip::CallError error) {
                logMessage(QString("Call failed to start, error: %1").arg((int)error));
            });
}

void
EchoCall::answer() {
    if (m_incomingCallUuid.isNull()) {
        logMessage("No incoming call.");
        return;
    }

    auto uuid = QUuid(std::move(m_incomingCallUuid));
    m_incomingCallUuid = QUuid();

    auto incomingCall = m_callee->findIncomingCall(uuid);
    if (!incomingCall) {
        logMessage("Incoming call not found within manager.");
        return;
    }

    incomingCall->answer(
            [this] {
                logMessage("Call is answered.");
            },
            [this](voip::CallError error) {
                logMessage(QString("Call failed to answer, error: %1").arg((int)error));
            });
}

void
EchoCall::logMessage(const QString &message) {
    // Currently we don't need real logging, just emit a signal
    Q_EMIT messageLogged(message);
}

void
EchoCall::outgoingCallPhaseChanged(std::shared_ptr<voip::Call> call, voip::CallPhase newPhase) {
    logMessage("EchoCall::outgoingCallPhaseChanged()");
}

void
EchoCall::outgoingCallConnectionStateChanged(std::shared_ptr<voip::Call> call,
        voip::CallConnectionState newConnectionState) {
    logMessage("EchoCall::outgoingCallConnectionStateChanged()");
}

void
EchoCall::outgoingCallFailed(std::shared_ptr<voip::Call> call, voip::CallError error) {
    logMessage(QString("EchoCall::outgoingCallFailed() error: %1").arg((int)error));
}

void
EchoCall::outgoingCreatedMessageToSent(voip::CallSignalingMessage *message) {
    logMessage("EchoCall::outgoingCreatedMessageToSent()");
    EchoCall::processCallSignalingMessage(*m_callee, *message);
    message->deleteLater();
}

void
EchoCall::incomingCallPhaseChanged(std::shared_ptr<voip::Call> call, voip::CallPhase newPhase) {
    logMessage("EchoCall::incomingCallPhaseChanged()");
}

void
EchoCall::incomingCallConnectionStateChanged(std::shared_ptr<voip::Call> call,
        voip::CallConnectionState newConnectionState) {
    logMessage("EchoCall::incomingCallConnectionStateChanged()");
}

void
EchoCall::incomingCallFailed(std::shared_ptr<voip::Call> call, voip::CallError error) {
    logMessage(QString("EchoCall::incomingCallFailed() error: %1").arg((int)error));
}

void
EchoCall::incomingCreatedMessageToSent(voip::CallSignalingMessage *message) {
    logMessage("EchoCall::incomingCreatedMessageToSent()");
    EchoCall::processCallSignalingMessage(*m_caller, *message);
    message->deleteLater();
}

void
EchoCall::processCallSignalingMessage(virgil::voip::CallManager &callManager,
        const virgil::voip::CallSignalingMessage &signalingMessage) {

    switch (signalingMessage.type()) {
    case voip::CallSignalingMessage::Type::callOffer: {
        const auto &callOffer = static_cast<const voip::CallOffer &>(signalingMessage);
        auto incomingCall = callManager.createIncomingCall(callOffer);
        incomingCall->start(
                [this, uuid = incomingCall->uuid()]() {
                    m_incomingCallUuid = std::move(uuid);
                },
                [this](voip::CallError error) {
                    logMessage(QString("Call failed to start, error: %1").arg((int)error));
                });
        break;
    }

    case voip::CallSignalingMessage::Type::callAnswer: {
        const auto &callAnswer = static_cast<const voip::CallAnswer &>(signalingMessage);
        callManager.processCallAnswer(callAnswer);
        break;
    }

    case voip::CallSignalingMessage::Type::iceCandidate: {
        const auto &iceCandidate = static_cast<const voip::IceCandidate &>(signalingMessage);
        callManager.processIceCandidate(iceCandidate);
        break;
    }

    case voip::CallSignalingMessage::Type::callUpdate:
        break;
    }
}
