#include "voip/CallAnswer.h"

#include <QJsonDocument>
#include <QDebug>

#include "voip/CallException.h"

using namespace virgil::voip;

CallAnswer::CallAnswer(std::string callUUID, std::string sdp)
    : CallSignalingMessage(std::move(callUUID)), sdp_(std::move(sdp)) {
}

CallAnswer::CallAnswer(std::string callUUID, std::string sdp, std::time_t createdAt)
    : CallSignalingMessage(std::move(callUUID), createdAt), sdp_(std::move(sdp)) {
}

CallSignalingMessage::Type
CallAnswer::type() const noexcept {
    return CallSignalingMessage::Type::callAnswer;
}

std::string
CallAnswer::sdp() const noexcept {
    return sdp_;
}
