#include "CallEnded.h"

#include "CallException.h"

using namespace virgil::voip;

CallSignalingMessage::Type
CallEnded::type() const noexcept {
    return CallSignalingMessage::Type::callEnded;
}
