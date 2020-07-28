#ifndef VIRGIL_VOIP_CALL_H_INCLUDED
#define VIRGIL_VOIP_CALL_H_INCLUDED

#include <QDateTime>
#include <QObject>
#include <QUuid>

#include <optional>

#include <webrtc/api/peer_connection_interface.h>

#include "CallError.h"
#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

enum class CallPhase {
    initial,  // the call was initiated
    calling,  // the call was initiated, but the caller is not available for now
    ringing,  // the caller is hearing the call
    accepted, // the caller has been accepted the call
    ended,    // call was ended
    failed
};

enum class CallConnectionStatus {
    none,         // Connection was not initiated yet
    initial,      // the call was initiated
    negotiating,  // esteblish connection parameters
    connected,    // connection stable
    disconnected, // temporary disconnected
    closed,       // connection was closed by one of the parties
    failed        // could not connect
};

/**
 * Base class for IncomingCall and OutgoingCall.
 */
class Call : public QObject, public webrtc::PeerConnectionObserver {
    Q_OBJECT

public:
    using OnSuccessFunc = std::function<void()>;
    using OnFailureFunc = std::function<void(CallError error)>;


    Call(QObject *parent, QString uuid, QString myName, QString otherName);
    virtual ~Call() = default;

    virtual bool
    isOutgoing() const noexcept = 0;

    void
    setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection);

    void
    end() noexcept;

    void
    hold(bool onHold) noexcept;

    QUuid
    uuid() const noexcept;

    QString
    myName() const noexcept;

    QString
    otherName() const noexcept;

    std::optional<QDateTime>
    connectedAt() const noexcept;

public:
    virtual void
    OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;

    virtual void
    OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

    virtual void
    OnRenegotiationNeeded() override;

    virtual void
    OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) override;

    virtual void
    OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

    virtual void
    OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;

Q_SIGNALS:
    void
    createdSignalingMessage(const CallSignalingMessage &message);

    void
    phaseChanged(CallPhase callPhase);

protected:
    /**
     *  Return defined peer contection or throws 'CallError::NoPeerConnection' otherwise.
     */
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>
    peerConnection();

    /**
     *  Return defined peer contection or throws 'CallError::NoPeerConnection' otherwise.
     */
    const rtc::scoped_refptr<webrtc::PeerConnectionInterface>
    peerConnection() const;

    void
    changePhase(CallPhase newPhase);

private:
    QUuid m_uuid;
    QString m_myName;
    QString m_otherName;

    CallPhase m_phase;
    std::optional<QDateTime> m_connectedAt;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_H_INCLUDED */