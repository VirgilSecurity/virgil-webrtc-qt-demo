#include "EchoCall.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include <stdexcept>

#include <voip/CallOffer.h>
#include <voip/CallAnswer.h>
#include <voip/IceCandidate.h>

namespace voip = virgil::voip;

static QString
generateUniqueId() {
    return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
}

static QString
messageTypeToString(voip::CallSignalingMessage::Type messageType) {
    switch (messageType) {
        case voip::CallSignalingMessage::Type::callOffer:
            return "call_offer";
        case voip::CallSignalingMessage::Type::callAnswer:
            return "call_answer";
        case voip::CallSignalingMessage::Type::callUpdate:
            return "call_update";
        case voip::CallSignalingMessage::Type::iceCandidate:
            return "ice_candidate";
        default:
            throw std::runtime_error("Unexpected message type");
    }
}

static voip::CallSignalingMessage::Type
messageTypeFromString(const QString &messageType) {

    if (messageType == "call_offer") {
        return voip::CallSignalingMessage::Type::callOffer;

    } else if (messageType == "call_answer") {
        return voip::CallSignalingMessage::Type::callAnswer;

    } else if (messageType == "call_update") {
        return voip::CallSignalingMessage::Type::callUpdate;

    } else if (messageType == "ice_candidate") {
        return voip::CallSignalingMessage::Type::iceCandidate;
    }

    throw std::runtime_error("Unexpected message type");
}

EchoCall::EchoCall(QObject *parent)
    : QObject(parent), m_callManager(std::make_unique<voip::CallManager>(generateUniqueId())) {

    //
    //  Connect Caller.
    //
    m_callManager->callPhaseChanged.connect([this](const voip::Call &call, voip::CallPhase phase) {
        this->callPhaseChanged(call, phase);
    });

    m_callManager->callConnectionStateChanged.connect(
            [this](const voip::Call &call, voip::CallConnectionState newState) {
                this->callConnectionStateChanged(call, newState);
            });

    m_callManager->createdMessageToSent.connect(
            [this](const voip::Call &call, const voip::CallSignalingMessage &signalingMessage) {
                this->callCreatedSignalingMessage(call, signalingMessage);
            });

    m_callManager->callFailed.connect([this](const voip::Call &call, voip::CallError error) {
        this->callFailed(call, error);
    });

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
    auto calee = generateUniqueId();
    auto call = m_callManager->createOutgoingCall(calee);
    call->start(
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

    auto incomingCall = m_callManager->findIncomingCall(uuid);
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
EchoCall::callPhaseChanged(const voip::Call &call, voip::CallPhase newPhase) {
    logMessage("EchoCall::callPhaseChanged()");
}

void
EchoCall::callConnectionStateChanged(const voip::Call &call, voip::CallConnectionState newConnectionState) {
    logMessage("EchoCall::callConnectionStateChanged()");
}

void
EchoCall::callFailed(const voip::Call &call, voip::CallError error) {
    logMessage(QString("EchoCall::callFailed() error: %1").arg((int)error));
}

void
EchoCall::callCreatedSignalingMessage(const voip::Call &call, const voip::CallSignalingMessage &message) {
    logMessage("EchoCall::callCreatedSignalingMessage()");
    this->sendCallSignalingMessage(message);
}

void
EchoCall::sendCallSignalingMessage(const voip::CallSignalingMessage &message) {

    //
    //  Serialize message.
    //
    auto json = QJsonObject();
    json["type"] = messageTypeToString(message.type());
    json["payload"] = message.toJson();
    auto jsonStr = QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact));

    //
    //  Send message.
    //
    auto sentBytes = m_socket.sendTextMessage(jsonStr);
    logMessage(QString("Sent %1 bytes to the socket").arg(sentBytes));
    m_socket.flush();
}

void
EchoCall::processCallSignalingMessage(const QString &messageString) {
    //
    //  Deserialize message.
    //
    QJsonParseError error{};
    auto jsonDocument = QJsonDocument::fromJson(messageString.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        logMessage(QString("Failed to parse signaling message (invalid JSON): %1").arg(error.errorString()));
        return;
    }

    if (!jsonDocument.isObject()) {
        logMessage(QString("Failed to parse signaling message (not an object)"));
        return;
    }

    auto json = jsonDocument.object();

    auto typeJson = json["type"];
    if (!typeJson.isString()) {
        logMessage(QString("Failed to parse signaling message (invalid or missed 'type' )"));
        return;
    }

    auto payloadJson = json["payload"];
    if (!payloadJson.isObject()) {
        logMessage(QString("Failed to parse signaling message (invalid or missed 'payload')"));
        return;
    }

    try {
        auto type = messageTypeFromString(typeJson.toString());
        switch (type) {
            case voip::CallSignalingMessage::Type::callOffer:
                processCallSignalingMessage(voip::CallOffer::fromJson(payloadJson.toObject()));
                break;

            case voip::CallSignalingMessage::Type::callAnswer:
                processCallSignalingMessage(voip::CallAnswer::fromJson(payloadJson.toObject()));
                break;

            case voip::CallSignalingMessage::Type::callUpdate:
                logMessage("Call updates are not handled for now");
                break;

            case voip::CallSignalingMessage::Type::iceCandidate:
                processCallSignalingMessage(voip::IceCandidate::fromJson(payloadJson.toObject()));
                break;

            default:
                throw std::runtime_error("Unexpected message type");
        }
    } catch (const std::exception &e) {
        logMessage(QString("Failed to parse signaling message: %1").arg(e.what()));
    }
}

void
EchoCall::processCallSignalingMessage(const voip::CallSignalingMessage &signalingMessage) {

    switch (signalingMessage.type()) {
        case voip::CallSignalingMessage::Type::callOffer: {
            const auto &callOffer = static_cast<const voip::CallOffer &>(signalingMessage);
            auto incomingCall = m_callManager->createIncomingCall(callOffer);
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
            m_callManager->processCallAnswer(callAnswer);
            break;
        }

        case voip::CallSignalingMessage::Type::iceCandidate: {
            const auto &iceCandidate = static_cast<const voip::IceCandidate &>(signalingMessage);
            m_callManager->processIceCandidate(iceCandidate);
            break;
        }

        case voip::CallSignalingMessage::Type::callUpdate:
            break;
    }
}

void
EchoCall::onSignalingMessageReceived(const QString &message) {
    logMessage("EchoCall::onSignalingMessageReceived()");
    qDebug() << message;
    processCallSignalingMessage(message);
}

void
EchoCall::onSocketConnected() {
    logMessage("EchoCall::onSocketConnected()");
    connect(&m_socket, &QWebSocket::textMessageReceived, this, &EchoCall::onSignalingMessageReceived);
}

void
EchoCall::onSocketDisconnected() {
    logMessage("EchoCall::onSocketDisconnected()");
    disconnect(&m_socket, &QWebSocket::textMessageReceived, this, &EchoCall::onSignalingMessageReceived);
}

void
EchoCall::onSocketError(QAbstractSocket::SocketError error) {
    logMessage(QString("EchoCall::onSocketError(): %1").arg(int(error)));
}
