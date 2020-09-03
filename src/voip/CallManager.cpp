#include "CallManager.h"

#include "Call.h"
#include "OutgoingCall.h"
#include "IncomingCall.h"
#include "CallConnectionFactory.h"
#include "PlatformCallManager.h"

using namespace virgil::voip;


CallManager::CallManager(std::string myId, const std::string &appName, std::unique_ptr<PlatformAudio> platformAudio)
    : myId_{std::move(myId)}, platformAudio_{std::move(platformAudio)}, calls_{} {

    if (!platformAudio_) {
        platformAudio_ = PlatformAudio::createDefault();
    }

    //
    //  Register.
    //
    PlatformCallManager::sharedInstance().tellSystemRegisterApplication(appName);
    connectPlatformCallManager();
}

const std::string &
CallManager::myId() const noexcept {
    return myId_;
}

void
CallManager::startOutgoingCall(std::string callUUID, std::string callee) {
    auto outgoingCall = std::make_shared<OutgoingCall>(std::move(callUUID), myId_, std::move(callee));

    this->connectCall(outgoingCall);

    calls_.push_back(outgoingCall);

    this->callCreated(*outgoingCall);

    PlatformCallManager::sharedInstance().tellSystemStartOutgoingCall(outgoingCall->uuid(), outgoingCall->otherName());
}

void
CallManager::startOutgoingCallFromSystem(std::string callUUID, std::string callee) {
    auto outgoingCall = std::make_shared<OutgoingCall>(std::move(callUUID), myId_, std::move(callee));

    this->connectCall(outgoingCall);

    calls_.push_back(outgoingCall);

    this->callCreated(*outgoingCall);

    outgoingCall->start();
}

void
CallManager::startIncomingCall(const CallOffer &callOffer) {
    auto incomingCall = std::make_shared<IncomingCall>(callOffer, myId_);

    this->connectCall(incomingCall);

    calls_.push_back(incomingCall);

    this->callCreated(*incomingCall);

    PlatformCallManager::sharedInstance().tellSystemStartIncomingCall(incomingCall->uuid(), incomingCall->otherName());
}

std::shared_ptr<Call>
CallManager::findCall(const std::string &uuid) noexcept {
    auto callIt = std::find_if(std::begin(calls_), std::end(calls_), [&uuid](const auto &call) {
        return call->uuid() == uuid;
    });

    if (callIt != std::end(calls_)) {
        return *callIt;
    } else {
        return nullptr;
    }
}


std::shared_ptr<IncomingCall>
CallManager::findIncomingCall(const std::string &uuid) noexcept {
    auto call = findCall(uuid);
    if (call && !call->isOutgoing()) {
        return std::static_pointer_cast<IncomingCall>(call);
    }

    return nullptr;
}

std::shared_ptr<OutgoingCall>
CallManager::findOutgoingCall(const std::string &uuid) noexcept {
    auto call = findCall(uuid);
    if (call && call->isOutgoing()) {
        return std::static_pointer_cast<OutgoingCall>(call);
    }

    return nullptr;
}

std::list<std::string>
CallManager::callUids() const {
    std::list<std::string> uids;
    for (const auto &call : calls_) {
        uids.emplace_back(call->uuid());
    }
    return uids;
}

void
CallManager::removeCall(const std::string &uuid) {
    std::remove_if(std::begin(calls_), std::end(calls_), [&uuid](const auto &call) {
        return call->uuid() == uuid;
    });
}

void
CallManager::processCallSignalingMessage(const CallSignalingMessage &callSignalingMessage) {
    auto type = callSignalingMessage.type();
    switch (type) {
        case voip::CallSignalingMessage::Type::callOffer:
            return processCallOffer(static_cast<const CallOffer &>(callSignalingMessage));

        case voip::CallSignalingMessage::Type::callAnswer:
            return processCallAnswer(static_cast<const CallAnswer &>(callSignalingMessage));

        case voip::CallSignalingMessage::Type::callReceived:
            return processCallReceived(static_cast<const CallReceived &>(callSignalingMessage));

        case voip::CallSignalingMessage::Type::callRejected:
            return processCallRejected(static_cast<const CallRejected &>(callSignalingMessage));

        case voip::CallSignalingMessage::Type::callEnded:
            return processCallEnded(static_cast<const CallEnded &>(callSignalingMessage));

        case voip::CallSignalingMessage::Type::iceCandidate:
            return processIceCandidate(static_cast<const IceCandidate &>(callSignalingMessage));

        default:
            throw std::runtime_error("Unexpected message type");
    }
}

void
CallManager::processCallOffer(const CallOffer &callOffer) {
    startIncomingCall(callOffer);
}

void
CallManager::processCallAnswer(const CallAnswer &callAnswer) {
    auto call = findOutgoingCall(callAnswer.callUUID());
    if (call) {
        call->accept(callAnswer);
    }
}

void
CallManager::processCallReceived(const CallReceived &callUpdate) {
    auto call = findOutgoingCall(callUpdate.callUUID());
    if (call) {
        call->update(callUpdate);
    }
}

void
CallManager::processCallRejected(const CallRejected &callUpdate) {
    auto call = findOutgoingCall(callUpdate.callUUID());
    if (call) {
        call->update(callUpdate);
    }
}

void
CallManager::processCallEnded(const CallEnded &callUpdate) {
    auto call = findCall(callUpdate.callUUID());
    if (call) {
        call->update(callUpdate);
    }
}

void
CallManager::processIceCandidate(const IceCandidate &iceCandidate) {
    auto call = findCall(iceCandidate.callUUID());
    if (call) {
        call->addRemoteIceCandidate(iceCandidate);
    }
}

void
CallManager::setMicrophoneOn(bool on) {
    for (const auto &call : calls_) {
        call->setMicrophoneOn(on);
    }
}

void
CallManager::setVoiceOn(bool on) {
    for (const auto &call : calls_) {
        call->setVoiceOn(on);
    }
}

bool
CallManager::hasSpeaker() const {
    return platformAudio_->hasSpeaker();
}

void
CallManager::setSpeakerOn(bool on) {
    platformAudio_->setSpeakerOn(on);
}

void
CallManager::setHoldOn(bool on) {
    for (const auto &call : calls_) {
        call->setHoldOn(on);
    }
}

void
CallManager::terminateAllCalls() {
    for (auto &call : calls_) {
        call->end();
    }
}

void
CallManager::connectPlatformCallManager() {

    auto &platformCallManager = PlatformCallManager::sharedInstance();

    //
    //  Handle signal: isReady.
    //
    slotConnections.emplace_back(platformCallManager.isReady.connect([this]() {
        // MAYBE we need to propagate this signal.
    }));

    //
    //  Handle signal: didRequestReset.
    //
    slotConnections.emplace_back(platformCallManager.didRequestReset.connect([this]() {
        this->terminateAllCalls();
    }));

    //
    //  Handle signal: didStartIncomingCall.
    //
    slotConnections.emplace_back(platformCallManager.didStartIncomingCall.connect([this](const std::string &callUUID) {
        auto incomingCall = findIncomingCall(callUUID);
        if (incomingCall) {
            incomingCall->start();
        } else {
            PlatformCallManager::sharedInstance().tellSystemEndCall(callUUID);
        }
    }));

    //
    //  Handle signal: didStartOutgoingCall.
    //
    slotConnections.emplace_back(platformCallManager.didStartOutgoingCall.connect([this](const std::string &callUUID) {
        auto outgoingCall = findOutgoingCall(callUUID);
        if (outgoingCall) {
            outgoingCall->start();
        } else {
            PlatformCallManager::sharedInstance().tellSystemEndCall(callUUID);
        }
    }));

    //
    //  Handle signal: didEndCall.
    //
    slotConnections.emplace_back(platformCallManager.didEndCall.connect([this](const std::string &callUUID) {
        auto call = findCall(callUUID);
        if (call) {
            // TODO: Maybe log this event.
        }
    }));

    //
    //  Handle signal: didFailStartCall.
    //
    slotConnections.emplace_back(platformCallManager.didFailStartCall.connect(
            [this](const std::string &callUUID, const std::string &details) {
                // TODO: log error details.
                (void)details;
                auto call = findCall(callUUID);
                if (call) {
                    call->die(CallError::FailedToStartSystemCall);
                }
            }));


    //
    //  Handle signal: didRequestCallStart.
    //
    slotConnections.emplace_back(platformCallManager.didRequestCallStart.connect(
            [this](const std::string &callUUID, const std::string &callee) {
                this->startOutgoingCallFromSystem(callUUID, callee);
            }));

    //
    //  Handle signal: didRequestCallAnswer.
    //
    slotConnections.emplace_back(platformCallManager.didRequestCallAnswer.connect([this](const std::string &callUUID) {
        auto incomingCall = findIncomingCall(callUUID);
        if (incomingCall) {
            incomingCall->answer();
        } else {
            PlatformCallManager::sharedInstance().tellSystemEndCall(callUUID);
        }
    }));

    //
    //  Handle signal: didRequestCallEnd.
    //
    slotConnections.emplace_back(platformCallManager.didRequestCallEnd.connect([this](const std::string &callUUID) {
        auto call = findCall(callUUID);
        if (call) {
            call->end();
        }
    }));

    //
    //  Handle signal: didRequestCallMute.
    //
    slotConnections.emplace_back(
            platformCallManager.didRequestCallMute.connect([this](const std::string &callUUID, bool onMute) {
            }));

    //
    //  Handle signal: didRequestCallHold.
    //
    slotConnections.emplace_back(
            platformCallManager.didRequestCallHold.connect([this](const std::string &callUUID, bool onHold) {
            }));

    //
    //  Handle signal: didActivateAudioSession.
    //
    slotConnections.emplace_back(platformCallManager.didActivateAudioSession.connect([this]() {
    }));

    //
    //  Handle signal: didDeactivateAudioSession.
    //
    slotConnections.emplace_back(platformCallManager.didDeactivateAudioSession.connect([this]() {
    }));
}

void
CallManager::connectCall(std::shared_ptr<Call> call) {
    call->started.connect([this, call]() {
        this->callStarted(*call);
    });

    call->received.connect([this, call]() {
        this->callReceived(*call);
    });

    call->accepted.connect([this, call]() {
        this->callAccepted(*call);
    });

    call->ended.connect([this, call](std::optional<CallError> maybeError) {
        this->callEnded(*call, maybeError);
        this->removeCall(call->uuid());
        PlatformCallManager::sharedInstance().tellSystemEndCall(call->uuid());
    });

    call->connectionStateChanged.connect([this, call](CallConnectionState newConnectionState) {
        this->callConnectionStateChanged(*call, newConnectionState);
    });

    call->sendSignalingMessage.connect([this, call](const CallSignalingMessage &signalingMessage) {
        this->sendSignalingMessage(*call, signalingMessage);
    });
}
