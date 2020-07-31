#ifndef ECHO_CALL_H_INCLUDED
#define ECHO_CALL_H_INCLUDED

#include <QObject>

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

    void
    outgoingCallPhaseChanged(const virgil::voip::Call &call, virgil::voip::CallPhase newPhase);

    void
    outgoingCallConnectionStateChanged(const virgil::voip::Call &call,
            virgil::voip::CallConnectionState newConnectionState);

    void
    outgoingCallFailed(const virgil::voip::Call &call, virgil::voip::CallError error);

    void
    outgoingCreatedMessageToSent(const virgil::voip::Call &call, const virgil::voip::CallSignalingMessage &message);

    void
    incomingCallPhaseChanged(const virgil::voip::Call &call, virgil::voip::CallPhase newPhase);

    void
    incomingCallConnectionStateChanged(const virgil::voip::Call &call,
            virgil::voip::CallConnectionState newConnectionState);

    void
    incomingCallFailed(const virgil::voip::Call &call, virgil::voip::CallError error);

    void
    incomingCreatedMessageToSent(const virgil::voip::Call &call, const virgil::voip::CallSignalingMessage &message);

Q_SIGNALS:
    void
    messageLogged(const QString &);

private:
    void
    logMessage(const QString &message);

    void
    processCallSignalingMessage(virgil::voip::CallManager &callManager,
            const virgil::voip::CallSignalingMessage &message);

private:
    std::unique_ptr<virgil::voip::CallManager> m_caller;
    std::unique_ptr<virgil::voip::CallManager> m_callee;
    QUuid m_incomingCallUuid;
};

#endif
