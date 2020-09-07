#include "SignalingServer.h"

#include <QDebug>

constexpr const size_t kReconnectTimout = 2000;

SignalingServer::SignalingServer(QObject *parent) : QObject(parent) {
    connect(this, &SignalingServer::start_, this, &SignalingServer::onStart);
    connect(this, &SignalingServer::processQuedMessages_, this, &SignalingServer::onProcessQuedMessages);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &SignalingServer::onReconnect);
}

void
SignalingServer::start() {
    Q_EMIT start_();
}

void
SignalingServer::onStart() {
    m_socket = new QWebSocket();

    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

    connect(m_socket, &QWebSocket::connected, this, &SignalingServer::onSocketConnected);
    connect(m_socket, &QWebSocket::disconnected, this, &SignalingServer::onSocketDisconnected);
    connect(m_socket, &QWebSocket::stateChanged, this, &SignalingServer::onSocketStateChanged);
    connect(m_socket, &QWebSocket::textMessageReceived, this, &SignalingServer::onMessageReceived);
    connect(m_socket,
            QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this,
            &SignalingServer::onSocketError);

    m_socket->open(QUrl("ws://165.232.68.63:8080"));
}

void
SignalingServer::onProcessQuedMessages() {
    if (m_queue.empty() || m_socket->state() != QAbstractSocket::SocketState::ConnectedState) {
        return;
    }

    auto message = m_queue.front();
    auto sentBytes = m_socket->sendTextMessage(message);
    m_queue.pop_front();

    qDebug() << QString("Sent %1 bytes to the socket").arg(sentBytes);

    m_socket->flush();
}

void
SignalingServer::sendMessage(const QString &message) {
    m_queue.push_back(message);

    Q_EMIT processQuedMessages_();
}

void
SignalingServer::onSocketConnected() {
    qDebug() << "Socket connected.";

    Q_EMIT connected();

    Q_EMIT processQuedMessages_();
}

void
SignalingServer::onSocketDisconnected() {
    qDebug() << "Socket disconnected.";

    Q_EMIT disconnected();
}

void
SignalingServer::onSocketStateChanged(QAbstractSocket::SocketState state) {
    qDebug() << "Socket state: " << int(state);

    if (state == QAbstractSocket::SocketState::ConnectingState) {
        qDebug() << "Socket connecting.";

        Q_EMIT connecting();
    }
}

void
SignalingServer::onMessageReceived(const QString &message) {
    Q_EMIT messageReceived(message);
}

void
SignalingServer::onSocketError(QAbstractSocket::SocketError) {
    qDebug() << "Socket error:" << m_socket->errorString();

    reconnect();
}

void
SignalingServer::onReconnect() {
    qDebug() << "Socket try to reconnect.";

    Q_EMIT tryReconnect();

    m_socket->abort();
    m_socket->open(QUrl("ws://165.232.68.63:8080"));
}

void
SignalingServer::reconnect() {
    m_reconnectTimer->start(kReconnectTimout);
}
