#include "EchoCall.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include <stdexcept>

#include <voip/CallOffer.h>
#include <voip/CallAnswer.h>
#include <voip/IceCandidate.h>

namespace voip = virgil::voip;

static std::string
generateUniqueId() {
    return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces).toStdString();
}

EchoCall::EchoCall(QObject *parent)
    : QObject(parent),
      m_callManager(std::make_unique<voip::CallManager>(generateUniqueId())),
      m_callAction(new Action(this)),
      m_answerAction(new Action(this)),
      m_endAction(new Action(this)),
      m_holdAction(new Action(this)),
      m_muteAction(new Action(this)),
      m_muteVoiceAction(new Action(this)),
      m_speakerAction(new Action(this)) {

    connect(m_callAction, &Action::triggered, this, &EchoCall::call);
    connect(m_answerAction, &Action::triggered, this, &EchoCall::answer);
    connect(m_endAction, &Action::triggered, this, &EchoCall::end);
    connect(m_holdAction, &Action::triggered, this, &EchoCall::hold);
    connect(m_muteAction, &Action::triggered, this, &EchoCall::mute);
    connect(m_muteVoiceAction, &Action::triggered, this, &EchoCall::muteVoice);
    connect(m_speakerAction, &Action::triggered, this, &EchoCall::speakerOnOff);

    //
    //  Connect CallManager events.
    //
    m_callManager->callCreated.connect([this](const voip::Call &call) {
        if (call.isOutgoing()) {
            this->logMessage("An outgoing call was created.");
        } else {
            this->logMessage("An incoming call was created.");
        }
        m_callAction->setEnabled(false);
    });

    m_callManager->callStarted.connect([this](const voip::Call &call) {
        if (call.isOutgoing()) {
            this->logMessage("An outgoing call was started.");
        } else {
            this->logMessage("An incoming call was started.");
            m_answerAction->setEnabled(true);
        }
        m_activeCallUuid = call.uuid();
        m_endAction->setEnabled(true);
    });

    m_callManager->callAccepted.connect([this](const voip::Call &call) {
        if (call.isOutgoing()) {
            this->logMessage("An outgoing call was accepted.");
        } else {
            this->logMessage("An incoming call was accepted.");
        }
    });

    m_callManager->callReceived.connect([this](const voip::Call &call) {
        if (call.isOutgoing()) {
            this->logMessage("Ringing...");

        } else {
            this->logMessage("An incoming call was received (SHOULD NOT HAPPEN).");
        }
    });

    m_callManager->callEnded.connect([this](const voip::Call &call, std::optional<voip::CallError> maybeError) {
        QString logMessage = "An";

        if (call.isOutgoing()) {
            logMessage += " outgoing";
        } else {
            logMessage += " incoming";
        }

        logMessage += " call was";

        switch (call.endReason()) {
            case voip::CallEndReason::ended:
                logMessage += " ended.";
                break;
            case voip::CallEndReason::rejected:
                logMessage += " rejected.";
                break;
            case voip::CallEndReason::failed:
                logMessage += " failed";
                break;
        }

        if (maybeError) {
            logMessage += QString(" with error: %1.").arg((int)(*maybeError));
        }

        this->logMessage(logMessage);

        m_activeCallUuid.clear();
        m_callAction->setEnabled(true);
        m_answerAction->setEnabled(false);
        m_endAction->setEnabled(false);
    });

    m_callManager->callConnectionStateChanged.connect(
            [this](const voip::Call &call, voip::CallConnectionState callConnectionState) {
                QString logMessage = "Call connection state: ";
                switch (callConnectionState) {
                    case voip::CallConnectionState::none:
                        logMessage += "none.";
                        break;
                    case voip::CallConnectionState::initial:
                        logMessage += "initial.";
                        break;
                    case voip::CallConnectionState::connecting:
                        logMessage += "connecting.";
                        break;
                    case voip::CallConnectionState::connected:
                        logMessage += "connected.";
                        break;
                    case voip::CallConnectionState::disconnected:
                        logMessage += "disconnected.";
                        break;
                    case voip::CallConnectionState::failed:
                        logMessage += "failed.";
                        break;
                    case voip::CallConnectionState::closed:
                        logMessage += "closed.";
                        break;
                }
                this->logMessage(logMessage);
            });

    m_callManager->sendSignalingMessage.connect(
            [this](const voip::Call &call, const voip::CallSignalingMessage &callSignalingMessage) {
                this->logMessage("Sending signaling message.");
                this->sendCallSignalingMessage(callSignalingMessage);
            });


    //
    //  Adjust UI.
    //
    m_speakerAction->setEnabled(m_callManager->hasSpeaker());
    m_answerAction->setEnabled(false);
    m_endAction->setEnabled(false);

    //
    //  Init socket.
    //
    connect(&m_socket, &QWebSocket::connected, this, &EchoCall::onSocketConnected);
    connect(&m_socket, &QWebSocket::disconnected, this, &EchoCall::onSocketDisconnected);
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &EchoCall::onSocketError);

    m_socket.open(QUrl("ws://165.232.68.63:8080"));
}


void
EchoCall::call() {
    auto callee = generateUniqueId();
    auto callUUID = generateUniqueId();
    m_callManager->startOutgoingCall(callUUID, callee);
}

void
EchoCall::answer() {
    if (m_activeCallUuid.empty()) {
        logMessage("No incoming call.");
        return;
    }

    auto incomingCall = m_callManager->findIncomingCall(m_activeCallUuid);
    if (!incomingCall) {
        logMessage("Incoming call not found within manager.");
        return;
    }

    m_answerAction->setEnabled(false);
    incomingCall->answer();
}

void
EchoCall::end() {
    if (m_activeCallUuid.empty()) {
        logMessage("No active call.");
        return;
    }

    auto activeCall = m_callManager->findCall(m_activeCallUuid);
    if (!activeCall) {
        logMessage("No active call to be ended.");
        return;
    }

    m_endAction->setEnabled(false);
    activeCall->end();
}

void
EchoCall::hold() {
    bool on = m_holdAction->checked();
    m_callManager->setHoldOn(on);

    if (on) {
        logMessage("Hold");
    } else {
        logMessage("Unhold");
    }
}

void
EchoCall::mute() {
    bool off = m_muteAction->checked();
    m_callManager->setMicrophoneOn(!off);

    if (off) {
        logMessage("Microphone was muted");
    } else {
        logMessage("Microphone was unmuted");
    }
}

void
EchoCall::speakerOnOff() {
    bool on = m_speakerAction->checked();
    m_callManager->setSpeakerOn(on);

    if (on) {
        logMessage("Speaker on");
    } else {
        logMessage("Speaker off");
    }
}

void
EchoCall::muteVoice() {
    bool off = m_muteVoiceAction->checked();
    m_callManager->setVoiceOn(!off);

    if (off) {
        logMessage("Voice was muted");
    } else {
        logMessage("Voice was unmuted");
    }
}

void
EchoCall::logMessage(const QString &message) {
    // Currently we don't need real logging, just emit a signal
    Q_EMIT messageLogged(message);
}

void
EchoCall::sendCallSignalingMessage(const voip::CallSignalingMessage &message) {

    //
    //  Serialize message.
    //
    auto messageStr = voip::CallSignalingMessage::toJsonString(message);

    //
    //  Send message.
    //
    auto sentBytes = m_socket.sendTextMessage(QString::fromStdString(messageStr));
    qDebug() << QString("Sent %1 bytes to the socket").arg(sentBytes);
    m_socket.flush();
}

void
EchoCall::processCallSignalingMessage(const QString &messageString) {
    logMessage("Processing signaling message...");
    qDebug() << messageString;
    try {
        auto signalingMessage = voip::CallSignalingMessage::fromJsonString(messageString.toStdString());
        m_callManager->processCallSignalingMessage(*signalingMessage);
    } catch (const std::exception &e) {
        logMessage(QString("Failed to parse signaling message: %1").arg(e.what()));
    }
}

void
EchoCall::onSignalingMessageReceived(const QString &message) {
    logMessage("Got signaling message.");
    processCallSignalingMessage(message);
}

void
EchoCall::onSocketConnected() {
    logMessage("Connected to the signaling server.");
    connect(&m_socket, &QWebSocket::textMessageReceived, this, &EchoCall::onSignalingMessageReceived);
}

void
EchoCall::onSocketDisconnected() {
    logMessage("Disconnected from the signaling server.");
    disconnect(&m_socket, &QWebSocket::textMessageReceived, this, &EchoCall::onSignalingMessageReceived);
}

void
EchoCall::onSocketError(QAbstractSocket::SocketError error) {
    logMessage(QString("Signaling server connection failed with socket error %1.").arg(int(error)));
}
