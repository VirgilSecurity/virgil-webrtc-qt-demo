#ifndef VIRGIL_VOIP_CALL_OFFER_H_INCLUDED
#define VIRGIL_VOIP_CALL_OFFER_H_INCLUDED

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class CallOffer : public CallSignalingMessage {
public:
    CallOffer(std::string callUUID, std::string caller, std::string sdp);

    CallOffer(std::string callUUID, std::string caller, std::string sdp, std::time_t createdAt);

    virtual CallSignalingMessage::Type
    type() const noexcept override;

    std::string
    caller() const noexcept;

    std::string
    sdp() const noexcept;

private:
    std::string caller_;
    std::string sdp_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_OFFER_H_INCLUDED */
