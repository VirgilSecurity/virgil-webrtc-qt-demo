#ifndef VIRGIL_VOIP_PLATFORM_CALL_MAANGER_IOS_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_CALL_MAANGER_IOS_H_INCLUDED

#include "PlatformCallManager.h"

#include <memory>

namespace virgil {
namespace voip {

//
// Implementation for iOS.
//
class PlatformCallManagerIOS : public PlatformCallManager {
public:
    class DelegateHelper;

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

    virtual bool
    tellSystemConfigureAudioSession() override;

    virtual bool
    tellSystemRestoreAudioSession() override;

private:
    PlatformCallManagerIOS();
    ~PlatformCallManagerIOS() noexcept;
    friend PlatformCallManager &
    PlatformCallManager::sharedInstance();

    friend class DelegateHelper;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_CALL_MAANGER_IOS_H_INCLUDED */
