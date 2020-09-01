#include "voip/IceCandidate.h"

#include <QJsonDocument>
#include <QDebug>

#include "voip/CallException.h"

using namespace virgil::voip;

IceCandidate::IceCandidate(std::string callUUID, int sdpMLineIndex, std::string sdpMid, std::string sdp)
    : CallSignalingMessage(std::move(callUUID)),
      sdpMLineIndex_(sdpMLineIndex),
      sdpMid_(std::move(sdpMid)),
      sdp_(std::move(sdp)) {
}

IceCandidate::IceCandidate(
        std::string callUUID,
        int sdpMLineIndex,
        std::string sdpMid,
        std::string sdp,
        std::time_t createdAt)
    : CallSignalingMessage(std::move(callUUID), createdAt),
      sdpMLineIndex_(sdpMLineIndex),
      sdpMid_(std::move(sdpMid)),
      sdp_(std::move(sdp)) {
}

CallSignalingMessage::Type
IceCandidate::type() const noexcept {
    return CallSignalingMessage::Type::iceCandidate;
}

int
IceCandidate::sdpMLineIndex() const noexcept {
    return sdpMLineIndex_;
}

std::string
IceCandidate::sdpMid() const noexcept {
    return sdpMid_;
}

std::string
IceCandidate::sdp() const noexcept {
    return sdp_;
}
