#ifndef VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED
#define VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED

#include "Call.h"
#include "IncomingCall.h"
#include "OutgoingCall.h"
#include "PlatformAudio.h"

#include <sigslot/signal.hpp>

#include <list>
#include <memory>
#include <string>


namespace virgil {
namespace voip {

class CallOffer;
class CallAnswer;
class IceCandidate;

//
//  Handles calls lifecycle and events.
//  Note, a call is added to the manager when created.
//  Note, a call is removed from the manager when it phase changed to the "ended" or "rejected".
//  Note, when a call is removed from the manager "callRemoved" event is fired.
//  Note, a failed call ia kept until it will be removed or recovered over update action.
//
class CallManager {
public:
    explicit CallManager(
            std::string myId,
            const ::std::string &appName,
            std::unique_ptr<PlatformAudio> platformAudio = nullptr);

    const std::string &
    myId() const noexcept;

    void
    startOutgoingCall(std::string uuid, std::string callee);

    void
    startIncomingCall(const CallOffer &callOffer);

    std::shared_ptr<Call>
    findCall(const std::string &uuid) noexcept;

    std::shared_ptr<IncomingCall>
    findIncomingCall(const std::string &uuid) noexcept;

    std::shared_ptr<OutgoingCall>
    findOutgoingCall(const std::string &uuid) noexcept;

    std::list<std::string>
    callUids() const;

    void
    processCallSignalingMessage(const CallSignalingMessage &callSignalingMessage);

    void
    setMicrophoneOn(bool on);

    void
    setVoiceOn(bool on);

    bool
    hasSpeaker() const;

    void
    setSpeakerOn(bool on);

    void
    setHoldOn(bool on);

    void
    terminateAllCalls();

    psigslot::signal<const Call &> callCreated;

    psigslot::signal<const Call &> callStarted;

    psigslot::signal<const Call &> callReceived;

    psigslot::signal<const Call &> callAccepted;

    psigslot::signal<const Call &, std::optional<CallError>> callEnded;

    psigslot::signal<const Call &, CallConnectionState> callConnectionStateChanged;

    psigslot::signal<const Call &, const CallSignalingMessage &> sendSignalingMessage;

private:
    void
    connectPlatformCallManager();

    void
    startOutgoingCallFromSystem(std::string uuid, std::string callee);

    void
    connectCall(std::shared_ptr<Call> call);

    void
    processCallOffer(const CallOffer &callOffer);

    void
    processCallAnswer(const CallAnswer &callAnswer);

    void
    processCallReceived(const CallReceived &callUpdate);

    void
    processCallRejected(const CallRejected &callUpdate);

    void
    processCallEnded(const CallEnded &callUpdate);

    void
    processIceCandidate(const IceCandidate &iceCandidate);

    void
    removeCall(const std::string &uuid);

private:
    std::string myId_;
    std::unique_ptr<PlatformAudio> platformAudio_;
    std::list<std::shared_ptr<Call>> calls_;
    std::list<psigslot::scoped_connection> slotConnections;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED */
