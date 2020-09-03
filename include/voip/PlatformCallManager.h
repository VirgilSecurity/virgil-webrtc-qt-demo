#ifndef VIRGIL_VOIP_PLATFORM_CALL_MAANGER_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_CALL_MAANGER_H_INCLUDED

#include <sigslot/signal.hpp>

#include <string>

namespace virgil {
namespace voip {

class PlatformCallManager {
public:
    static PlatformCallManager &
    sharedInstance();

    virtual void
    tellSystemRegisterApplication(const std::string &appName) = 0;

    //
    //  Should fire one of the signals:
    //    - didStartIncomingCall;
    //    - didFailCall.
    //
    virtual void
    tellSystemStartIncomingCall(const std::string &callUUID, const std::string &caller) = 0;

    //
    //  Should fire one of the signals:
    //    - didStartOutgoingCall;
    //    - didFailCall.
    //
    virtual void
    tellSystemStartOutgoingCall(const std::string &callUUID, const std::string &callee) = 0;

    virtual void
    tellSystemCallConnected(const std::string &callUUID) = 0;

    //
    //  Should fire one of the signals:
    //    - didEndCall.
    //
    virtual void
    tellSystemEndCall(const std::string &callUUID) = 0;

    //
    //  Should not fire any signals.
    //
    virtual void
    tellSystemDummyIncomingCall() = 0;

    virtual void
    tellSystemMuteCall(const std::string &callUUID, bool onMute) = 0;

    virtual void
    tellSystemHoldCall(const std::string &callUUID, bool onHold) = 0;

    virtual ~PlatformCallManager() noexcept = default;

public:
    psigslot::signal<> isReady;

    psigslot::signal<> didActivateAudioSession;

    psigslot::signal<> didDeactivateAudioSession;

    psigslot::signal<const std::string &> didStartIncomingCall;

    psigslot::signal<const std::string &> didStartOutgoingCall;

    psigslot::signal<const std::string &> didEndCall;

    psigslot::signal<const std::string &, const std::string &> didFailStartCall;

    psigslot::signal<> didRequestReset;

    psigslot::signal<const std::string &> didRequestCallAnswer;

    psigslot::signal<const std::string &> didRequestCallEnd;

    psigslot::signal<const std::string &, bool> didRequestCallMute;

    psigslot::signal<const std::string &, bool> didRequestCallHold;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_CALL_MAANGER_H_INCLUDED */
