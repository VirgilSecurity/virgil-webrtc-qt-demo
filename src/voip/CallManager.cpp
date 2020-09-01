#include "voip/CallManager.h"

#include "Call.h"
#include "OutgoingCall.h"
#include "IncomingCall.h"
#include "CallConnectionFactory.h"

using namespace virgil::voip;


CallManager::CallManager(std::string myId, std::unique_ptr<PlatformAudio> platformAudio)
    : myId_(std::move(myId)), platformAudio_(std::move(platformAudio)) {
    if (!platformAudio_) {
        platformAudio_ = PlatformAudio::createDefault();
    }
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

    outgoingCall->start();
}

void
CallManager::startIncomingCall(const CallOffer &callOffer) {
    auto incomingCall = std::make_shared<IncomingCall>(callOffer, myId_);

    this->connectCall(incomingCall);

    calls_.push_back(incomingCall);

    this->callCreated(*incomingCall);

    incomingCall->start();
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
    auto call = findOutgoingCall(callUpdate.callUUID());
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
    });

    call->connectionStateChanged.connect([this, call](CallConnectionState newConnectionState) {
        this->callConnectionStateChanged(*call, newConnectionState);
    });

    call->sendSignalingMessage.connect([this, call](const CallSignalingMessage &signalingMessage) {
        this->sendSignalingMessage(*call, signalingMessage);
    });
}
