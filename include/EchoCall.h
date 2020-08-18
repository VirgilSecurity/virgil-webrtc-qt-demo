#ifndef ECHO_CALL_H_INCLUDED
#define ECHO_CALL_H_INCLUDED

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QUuid>

#include <memory>

#include <voip/CallManager.h>
#include <sigslot/signal.hpp>

class EchoCall : public QObject {
    Q_OBJECT

public:
    explicit EchoCall(QObject *parent);

    Q_INVOKABLE void
    call();

    Q_INVOKABLE void
    answer();

Q_SIGNALS:
    void
    messageLogged(const QString &);

private:
    void
    callPhaseChanged(const virgil::voip::Call &call, virgil::voip::CallPhase newPhase);

    void
    callConnectionStateChanged(const virgil::voip::Call &call, virgil::voip::CallConnectionState newConnectionState);

    void
    callFailed(const virgil::voip::Call &call, virgil::voip::CallError error);

    void
    callCreatedSignalingMessage(const virgil::voip::Call &call, const virgil::voip::CallSignalingMessage &message);

    void
    onSignalingMessageReceived(const QString &message);

    void
    onSocketConnected();

    void
    onSocketDisconnected();

    void
    onSocketError(QAbstractSocket::SocketError error);

private:
    void
    logMessage(const QString &message);

    void
    sendCallSignalingMessage(const virgil::voip::CallSignalingMessage &message);

    void
    processCallSignalingMessage(const QString &messageString);

    void
    processCallSignalingMessage(const virgil::voip::CallSignalingMessage &message);

private:
    std::unique_ptr<virgil::voip::CallManager> m_callManager;
    QUuid m_incomingCallUuid;
    QWebSocket m_socket;
};

#endif
