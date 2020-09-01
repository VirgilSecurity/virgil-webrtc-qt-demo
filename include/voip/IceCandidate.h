#ifndef VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED
#define VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class IceCandidate : public CallSignalingMessage {
public:
    IceCandidate(std::string callUUID, int sdpMLineIndex, std::string sdpMid, std::string sdp);

    IceCandidate(std::string callUUID, int sdpMLineIndex, std::string sdpMid, std::string sdp, std::time_t createdAt);

    virtual CallSignalingMessage::Type
    type() const noexcept override;

    int
    sdpMLineIndex() const noexcept;

    std::string
    sdpMid() const noexcept;

    std::string
    sdp() const noexcept;

private:
    int sdpMLineIndex_;
    std::string sdpMid_;
    std::string sdp_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_ICE_CANDIDATE_H_INCLUDED */
