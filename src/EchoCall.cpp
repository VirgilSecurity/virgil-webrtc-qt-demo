#include "EchoCall.h"

#include <QDebug>

namespace voip = virgil::voip;

EchoCall::EchoCall(QObject *parent)
    : QObject(parent),
      m_caller(std::make_unique<voip::CallManager>()),
      m_callee(std::make_unique<voip::CallManager>()) {

    qRegisterMetaType<voip::CallPhase>("CallPhase");
    qRegisterMetaType<voip::CallConnectionState>("CallConnectionState");


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
}


void
EchoCall::call() {
    auto outgoingCall = m_caller->createOutgoingCall("its_me");
    outgoingCall->start(
            [this]() {
                logMessage("Call started...");
            },
            [this](voip::CallError error) {
                logMessage("Call failed to start...");
            });
}

void
EchoCall::answer() {
    logMessage("Not implemented...");
}

void
EchoCall::logMessage(const QString &message) {
    // Currently we don't need real logging, just emit a signal
    Q_EMIT messageLogged(message);
}

void
EchoCall::outgoingCallPhaseChanged(std::shared_ptr<voip::Call> call, voip::CallPhase newPhase) {
    qDebug() << "EchoCall::outgoingCallPhaseChanged()" << call->uuid().toString();
}

void
EchoCall::outgoingCallConnectionStateChanged(std::shared_ptr<voip::Call> call,
        voip::CallConnectionState newConnectionState) {
    qDebug() << "EchoCall::outgoingCallConnectionStateChanged()";
}

void
EchoCall::outgoingCreatedMessageToSent(std::shared_ptr<voip::CallSignalingMessage> message) {
    qDebug() << "EchoCall::outgoingCreatedMessageToSent()";
}

void
EchoCall::incomingCallPhaseChanged(std::shared_ptr<voip::Call> call, voip::CallPhase newPhase) {
    qDebug() << "EchoCall::incomingCallPhaseChanged()";
}

void
EchoCall::incomingCallConnectionStateChanged(std::shared_ptr<voip::Call> call,
        voip::CallConnectionState newConnectionState) {
    qDebug() << "EchoCall::incomingCallConnectionStateChanged()";
}

void
EchoCall::incomingCreatedMessageToSent(std::shared_ptr<voip::CallSignalingMessage> message) {
    qDebug() << "EchoCall::incomingCreatedMessageToSent()";
}
