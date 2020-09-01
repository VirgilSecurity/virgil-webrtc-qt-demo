#ifndef ECHO_CALL_H_INCLUDED
#define ECHO_CALL_H_INCLUDED

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QUuid>

#include <memory>

#include <voip/CallManager.h>
#include <sigslot/signal.hpp>

class Action : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)


public:
    explicit Action(QObject *parent = nullptr) : QObject(parent) {
    }
    virtual ~Action() {
    }

    bool
    visible() const {
        return m_visible;
    }
    bool
    enabled() const {
        return m_enabled;
    }
    bool
    checked() const {
        return m_checked;
    }

    void
    setVisible(bool visible) {
        if (m_visible != visible) {
            m_visible = visible;
            Q_EMIT visibleChanged(visible);
        }
    }
    void
    setEnabled(bool enabled) {
        if (m_enabled != enabled) {
            m_enabled = enabled;
            Q_EMIT enabledChanged(enabled);
        }
    }
    void
    setChecked(bool checked) {
        if (m_checked != checked) {
            m_checked = checked;
            Q_EMIT checkedChanged(checked);
        }
    }

    Q_INVOKABLE void
    trigger() {
        Q_EMIT triggered();
    }

Q_SIGNALS:
    void
    visibleChanged(bool);
    void
    enabledChanged(bool);
    void
    checkedChanged(bool);

    void
    triggered();

private:
    bool m_visible = true;
    bool m_enabled = true;
    bool m_checked = false;
};

class EchoCall : public QObject {
    Q_OBJECT
    Q_PROPERTY(Action *callAction MEMBER m_callAction CONSTANT)
    Q_PROPERTY(Action *answerAction MEMBER m_answerAction CONSTANT)
    Q_PROPERTY(Action *endAction MEMBER m_endAction CONSTANT)
    Q_PROPERTY(Action *holdAction MEMBER m_holdAction CONSTANT)
    Q_PROPERTY(Action *muteAction MEMBER m_muteAction CONSTANT)
    Q_PROPERTY(Action *speakerAction MEMBER m_speakerAction CONSTANT)
    Q_PROPERTY(Action *muteVoiceAction MEMBER m_muteVoiceAction CONSTANT)

public:
    explicit EchoCall(QObject *parent);

    void
    call();

    void
    answer();

    void
    end();

    void
    hold();

    void
    mute();

    void
    muteVoice();

    void
    speakerOnOff();

Q_SIGNALS:
    void
    messageLogged(const QString &);

private:
    void
    logMessage(const QString &message);

    void
    sendCallSignalingMessage(const virgil::voip::CallSignalingMessage &message);

    void
    processCallSignalingMessage(const QString &messageString);

private:
    void
    onSignalingMessageReceived(const QString &message);

    void
    onSocketConnected();

    void
    onSocketDisconnected();

    void
    onSocketError(QAbstractSocket::SocketError error);

private:
    std::unique_ptr<virgil::voip::CallManager> m_callManager;
    std::string m_activeCallUuid;
    QWebSocket m_socket;

    Action *m_callAction;
    Action *m_answerAction;
    Action *m_endAction;
    Action *m_holdAction;
    Action *m_muteAction;
    Action *m_muteVoiceAction;
    Action *m_speakerAction;
};

#endif
