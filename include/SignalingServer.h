#ifndef SIGNALING_SERVER_H_INCLUDED
#define SIGNALING_SERVER_H_INCLUDED

#include <QPointer>
#include <QQueue>
#include <QString>
#include <QTimer>
#include <QWebSocket>

class SignalingServer : public QObject {
    Q_OBJECT

Q_SIGNALS:
    void
    connecting();

    void
    connected();

    void
    disconnected();

    void
    tryReconnect();

    void
    messageReceived(const QString &message);

    void
    processQuedMessages_();

    void
    start_();

public:
    explicit SignalingServer(QObject *parent = nullptr);

    void
    start();

public Q_SLOTS:
    void
    sendMessage(const QString &message);

private Q_SLOTS:
    void
    onStart();

    void
    onProcessQuedMessages();

    void
    onMessageReceived(const QString &message);

    void
    onSocketStateChanged(QAbstractSocket::SocketState state);

    void
    onSocketConnected();

    void
    onSocketDisconnected();

    void
    onSocketError(QAbstractSocket::SocketError error);

    void
    onReconnect();

private:

    void
    reconnect();

private:
    QQueue<QString> m_queue;
    QPointer<QWebSocket> m_socket;
    QTimer* m_reconnectTimer;
};

#endif // SIGNALING_SERVER_H_INCLUDED
