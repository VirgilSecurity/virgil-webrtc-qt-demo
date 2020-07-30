#ifndef ECHO_CALL_H_INCLUDED
#define ECHO_CALL_H_INCLUDED

#include <QObject>

#include <memory>

#include <voip/CallManager.h>

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

public Q_SLOTS:
    void
    outgoingCallPhaseChanged(std::shared_ptr<virgil::voip::Call> call, virgil::voip::CallPhase newPhase);

    void
    outgoingCallConnectionStateChanged(std::shared_ptr<virgil::voip::Call> call,
            virgil::voip::CallConnectionState newConnectionState);

    void
    outgoingCallFailed(std::shared_ptr<virgil::voip::Call> call, virgil::voip::CallError error);

    void
    outgoingCreatedMessageToSent(virgil::voip::CallSignalingMessage* message);

    void
    incomingCallPhaseChanged(std::shared_ptr<virgil::voip::Call> call, virgil::voip::CallPhase newPhase);

    void
    incomingCallConnectionStateChanged(std::shared_ptr<virgil::voip::Call> call,
            virgil::voip::CallConnectionState newConnectionState);

    void
    incomingCallFailed(std::shared_ptr<virgil::voip::Call> call, virgil::voip::CallError error);

    void
    incomingCreatedMessageToSent(virgil::voip::CallSignalingMessage* message);

private:
    void
    logMessage(const QString &message);

    void
    processCallSignalingMessage(virgil::voip::CallManager& callManager,
        const virgil::voip::CallSignalingMessage& message);

private:
    std::unique_ptr<virgil::voip::CallManager> m_caller;
    std::unique_ptr<virgil::voip::CallManager> m_callee;
    QUuid m_incomingCallUuid;
};

#endif
