#include "PlatformCallManagerDefault.h"

using namespace virgil::voip;

PlatformCallManager &
PlatformCallManager::sharedInstance() {
    static PlatformCallManagerDefault instance{};
    return instance;
}

void
PlatformCallManagerDefault::tellSystemRegisterApplication(const std::string &appName) {
    (void)appName;
    this->isReady();
}

void
PlatformCallManagerDefault::tellSystemStartIncomingCall(const std::string &callUUID, const std::string &caller) {
    (void)caller;
    this->didStartIncomingCall(callUUID);
}

void
PlatformCallManagerDefault::tellSystemStartOutgoingCall(const std::string &callUUID, const std::string &callee) {
    (void)callee;
    this->didStartOutgoingCall(callUUID);
}

void
PlatformCallManagerDefault::tellSystemCallConnected(const std::string &callUUID) {
    (void)callUUID;
}

void
PlatformCallManagerDefault::tellSystemEndCall(const std::string &callUUID) {
    this->didEndCall(callUUID);
}

void
PlatformCallManagerDefault::tellSystemDummyIncomingCall() {
}

void
PlatformCallManagerDefault::tellSystemMuteCall(const std::string &callUUID, bool onMute) {
    // FIXME: What to do here?
}

void
PlatformCallManagerDefault::tellSystemHoldCall(const std::string &callUUID, bool onHold) {
    // FIXME: What to do here?
}
