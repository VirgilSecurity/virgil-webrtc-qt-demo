#include "CallRejected.h"

#include "CallException.h"

using namespace virgil::voip;

CallSignalingMessage::Type
CallRejected::type() const noexcept {
    return CallSignalingMessage::Type::callRejected;
}
