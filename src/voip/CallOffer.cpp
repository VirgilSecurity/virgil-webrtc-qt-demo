#include "CallOffer.h"

#include "CallException.h"

using namespace virgil::voip;

CallOffer::CallOffer(std::string callUUID, std::string caller, std::string sdp)
    : CallSignalingMessage(std::move(callUUID)), caller_(std::move(caller)), sdp_(std::move(sdp)) {
}

CallOffer::CallOffer(std::string callUUID, std::string caller, std::string sdp, std::time_t createdAt)
    : CallSignalingMessage(std::move(callUUID), createdAt), caller_(std::move(caller)), sdp_(std::move(sdp)) {
}

CallSignalingMessage::Type
CallOffer::type() const noexcept {
    return CallSignalingMessage::Type::callOffer;
}

std::string
CallOffer::caller() const noexcept {
    return caller_;
}

std::string
CallOffer::sdp() const noexcept {
    return sdp_;
}
