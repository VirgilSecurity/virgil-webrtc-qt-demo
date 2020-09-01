#ifndef VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED
#define VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class CallAnswer : public CallSignalingMessage {
public:
    CallAnswer(std::string callUUID, std::string sdp);

    CallAnswer(std::string callUUID, std::string sdp, std::time_t createdAt);

    virtual CallSignalingMessage::Type
    type() const noexcept override;

    std::string
    sdp() const noexcept;

private:
    std::string sdp_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_ANSWER_H_INCLUDED */
