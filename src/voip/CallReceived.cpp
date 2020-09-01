#include "CallReceived.h"

#include "CallException.h"

using namespace virgil::voip;

CallSignalingMessage::Type
CallReceived::type() const noexcept {
    return CallSignalingMessage::Type::callReceived;
}
