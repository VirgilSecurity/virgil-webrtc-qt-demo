#ifndef VIRGIL_VOIP_CALL_RECEIVED_H_INCLUDED
#define VIRGIL_VOIP_CALL_RECEIVED_H_INCLUDED

#include "CallSignalingMessage.h"

namespace virgil {
namespace voip {

class CallReceived : public CallSignalingMessage {
public:
    using CallSignalingMessage::CallSignalingMessage;

    virtual CallSignalingMessage::Type
    type() const noexcept override;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_RECEIVED_H_INCLUDED */
