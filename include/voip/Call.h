#ifndef VIRGIL_VOIP_CALL_H_INCLUDED
#define VIRGIL_VOIP_CALL_H_INCLUDED

#include <QDateTime>
#include <QObject>
#include <QUuid>

#include <optional>

#include <webrtc/api/peer_connection_interface.h>
#include <sigslot/signal.hpp>

#include "CallError.h"
#include "CallSignalingMessage.h"
#include "IceCandidate.h"


namespace virgil {
namespace voip {

enum class CallPhase {
    initial,  // the call was initiated
    calling,  // the call was initiated, but the caller is not available for now
    ringing,  // the caller is hearing the call
    accepted, // the caller has been accepted the call
    ended,    // call was ended
    failed,
};

enum class CallConnectionState {
    none,         // Connection was not initiated yet
    initial,      // the call was initiated
    connecting,   // esteblish connection parameters
    connected,    // connection stable
    disconnected, // temporary disconnected
    failed,       // could not connect
    closed,       // connection was closed by one of the parties
};

/**
 * Base class for IncomingCall and OutgoingCall.
 */
class Call : public webrtc::PeerConnectionObserver {
public:
    using OnSuccessFunc = std::function<void()>;
    using OnFailureFunc = std::function<void(CallError error)>;


    Call(QUuid uuid, QString myName, QString otherName);
    virtual ~Call() = default;

    virtual bool
    isOutgoing() const noexcept = 0;

    void
    setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection);

    void
    addRemoteIceCandidate(const IceCandidate &iceCandidate);

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
    OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) override;

    virtual void
    OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

    virtual void
    OnRenegotiationNeeded() override;

    virtual void
    OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState newState) override;

    virtual void
    OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) override;

    virtual void
    OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;

    psigslot::signal<CallPhase> phaseChanged;

    psigslot::signal<CallConnectionState> connectionStateChanged;

    psigslot::signal<const CallSignalingMessage &> createdSignalingMessage;

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
    changePhase(CallPhase newPhase) noexcept;

    void
    changeConnectionState(CallConnectionState newState) noexcept;

private:
    QUuid m_uuid;
    QString m_myName;
    QString m_otherName;

    CallPhase m_phase;
    CallConnectionState m_connectionState;

    std::optional<QDateTime> m_connectedAt;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_H_INCLUDED */
