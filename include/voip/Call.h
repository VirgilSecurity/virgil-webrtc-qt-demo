#ifndef VIRGIL_VOIP_CALL_H_INCLUDED
#define VIRGIL_VOIP_CALL_H_INCLUDED

#include "CallError.h"
#include "CallSignalingMessage.h"
#include "IceCandidate.h"
#include "CallReceived.h"
#include "CallRejected.h"
#include "CallEnded.h"

#include <webrtc/api/peer_connection_interface.h>
#include <webrtc/rtc_base/critical_section.h>
#include <webrtc/rtc_base/thread_checker.h>

#include <sigslot/signal.hpp>

#include <optional>
#include <vector>
#include <ctime>

namespace virgil {
namespace voip {

enum class CallPhase {
    initial,  // a call was initiated
    started,  // a call was started
    received, // a caller is hearing a call
    accepted, // a caller has been accepted a call
    ended,    // a call was ended after acceptance
};

enum class CallEndReason {
    ended,    // a call was ended after be accepted
    rejected, // a call was ended before be accepted
    failed,   // a call was failed
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
    Call(std::string uuid, std::string myName, std::string otherName);
    virtual ~Call() = default;

    virtual bool
    isOutgoing() const noexcept = 0;

    void
    setupPeerConnection(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection);

    void
    addRemoteIceCandidate(const IceCandidate &iceCandidate);

    void
    update(const CallReceived &callReceived);

    void
    update(const CallRejected &callRejected);

    void
    update(const CallEnded &callEnded);

    void
    end(std::optional<CallError> maybeError = std::optional<CallError>());

    void
    setHoldOn(bool on);

    void
    setMicrophoneOn(bool on);

    void
    setVoiceOn(bool on);

    std::string
    uuid() const noexcept;

    std::string
    myName() const noexcept;

    std::string
    otherName() const noexcept;

    std::optional<std::time_t>
    connectedAt() const noexcept;

    CallPhase
    phase() const noexcept;

    CallEndReason
    endReason() const noexcept;

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

    psigslot::signal<> started;

    psigslot::signal<> received;

    psigslot::signal<> accepted;

    psigslot::signal<std::optional<CallError>> ended;

    psigslot::signal<CallConnectionState> connectionStateChanged;

    psigslot::signal<const CallSignalingMessage &> sendSignalingMessage;

protected:
    void
    die(CallError error);

    void
    changePhase(CallPhase newPhase, std::optional<CallError> maybeError = {}) noexcept;

    void
    changeConnectionState(CallConnectionState newState) noexcept;

    void
    doPeerConnectionOp(std::function<void(rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection_)> op);

private:
    void
    die(CallEndReason endReason, std::optional<CallError> maybeError = {});

private:
    std::string uuid_;
    std::string myName_;
    std::string otherName_;

    CallPhase phase_;
    CallEndReason endReason_;
    CallConnectionState connectionState_;

    std::optional<std::time_t> connectedAt_;

    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_H_INCLUDED */
