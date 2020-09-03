#ifndef VIRGIL_VOIP_PLATFORM_CALL_MAANGER_DEFAULT_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_CALL_MAANGER_DEFAULT_H_INCLUDED

#include "PlatformCallManager.h"

namespace virgil {
namespace voip {

//
// Default implementation.
// Call required "success" signals immediately.
//
class PlatformCallManagerDefault : public PlatformCallManager {
public:
    virtual void
    tellSystemRegisterApplication(const std::string &appName) override;

    virtual void
    tellSystemStartIncomingCall(const std::string &callUUID, const std::string &caller) override;

    virtual void
    tellSystemStartOutgoingCall(const std::string &callUUID, const std::string &callee) override;

    virtual void
    tellSystemCallConnected(const std::string &callUUID) override;

    virtual void
    tellSystemEndCall(const std::string &callUUID) override;

    virtual void
    tellSystemDummyIncomingCall() override;

    virtual void
    tellSystemMuteCall(const std::string &callUUID, bool onMute) override;

    virtual void
    tellSystemHoldCall(const std::string &callUUID, bool onHold) override;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_CALL_MAANGER_DEFAULT_H_INCLUDED */
