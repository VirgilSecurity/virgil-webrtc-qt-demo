#ifndef VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED
#define VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED

#include <ctime>
#include <string>
#include <memory>

namespace virgil {
namespace voip {

/**
 * Base class for call signaling messeges.
 */
class CallSignalingMessage {
public:
    enum class Type {
        callOffer,
        callAnswer,
        callReceived,
        callRejected,
        callEnded,
        iceCandidate,
    };

public:
    explicit CallSignalingMessage(std::string callUUID);

    CallSignalingMessage(std::string callUUID, std::time_t createdAt);

    virtual Type
    type() const = 0;

    virtual ~CallSignalingMessage() noexcept = default;

    std::string
    callUUID() const noexcept;

    std::time_t
    createdAt() const noexcept;

    static std::string
    toJsonString(const CallSignalingMessage &callSignalingMessage);

    static std::unique_ptr<CallSignalingMessage>
    fromJsonString(const std::string &jsonStr);

private:
    std::string callUUID_;
    std::time_t createdAt_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED */
