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
    createdAt() const noexcept;

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

private:
    QUuid m_uuid;
    QString m_myName;
    QString m_otherName;

    std::optional<QDateTime> m_createdAt;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_H_INCLUDED */
