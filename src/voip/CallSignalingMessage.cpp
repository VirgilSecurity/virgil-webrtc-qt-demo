#include "CallSignalingMessage.h"

#include "CallOffer.h"
#include "CallAnswer.h"
#include "CallReceived.h"
#include "CallRejected.h"
#include "CallEnded.h"
#include "IceCandidate.h"
#include "CallException.h"

#include <nlohmann/json.hpp>

constexpr const char kJsonKey_Type[] = "type";
constexpr const char kJsonKey_Payload[] = "payload";
constexpr const char kJsonKey_CallUUID[] = "call_uuid";
constexpr const char kJsonKey_CreatedAt[] = "created_at";
constexpr const char kJsonKey_Caller[] = "caller";
constexpr const char kJsonKey_SdpMLineIndex[] = "sdp_m_line_index";
constexpr const char kJsonKey_SdpMid[] = "sdp_mid";
constexpr const char kJsonKey_Sdp[] = "sdp";

constexpr const char kJsonValue_CallOffer[] = "call_offer";
constexpr const char kJsonValue_CallAnswer[] = "call_answer";
constexpr const char kJsonValue_CallReceived[] = "call_received";
constexpr const char kJsonValue_CallRejected[] = "call_rejected";
constexpr const char kJsonValue_CallEnded[] = "call_ended";
constexpr const char kJsonValue_IceCandidate[] = "ice_candidate";

namespace nlohmann {
//
// virgil::voip::CallOffer.
//
template <> struct adl_serializer<virgil::voip::CallOffer> {
    static void
    to_json(json &j, const virgil::voip::CallOffer &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
        j[kJsonKey_Caller] = t.caller();
        j[kJsonKey_Sdp] = t.sdp();
    }

    static virgil::voip::CallOffer
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();
        auto caller = j.at(kJsonKey_Caller).get<std::string>();
        auto sdp = j.at(kJsonKey_Sdp).get<std::string>();

        return {callUUID, caller, sdp, createdAt};
    }
};

//
// virgil::voip::CallAnswer.
//
template <> struct adl_serializer<virgil::voip::CallAnswer> {
    static void
    to_json(json &j, const virgil::voip::CallAnswer &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
        j[kJsonKey_Sdp] = t.sdp();
    }

    static virgil::voip::CallAnswer
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();
        auto sdp = j.at(kJsonKey_Sdp).get<std::string>();

        return {callUUID, sdp, createdAt};
    }
};

//
// virgil::voip::CallReceived.
//
template <> struct adl_serializer<virgil::voip::CallReceived> {
    static void
    to_json(json &j, const virgil::voip::CallReceived &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
    }

    static virgil::voip::CallReceived
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();

        return {callUUID, createdAt};
    }
};

//
// virgil::voip::CallRejected.
//
template <> struct adl_serializer<virgil::voip::CallRejected> {
    static void
    to_json(json &j, const virgil::voip::CallRejected &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
    }

    static virgil::voip::CallRejected
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();

        return {callUUID, createdAt};
    }
};

//
// virgil::voip::CallEnded.
//
template <> struct adl_serializer<virgil::voip::CallEnded> {
    static void
    to_json(json &j, const virgil::voip::CallEnded &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
    }

    static virgil::voip::CallEnded
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();

        return {callUUID, createdAt};
    }
};

//
// virgil::voip::IceCandidate.
//
template <> struct adl_serializer<virgil::voip::IceCandidate> {
    static void
    to_json(json &j, const virgil::voip::IceCandidate &t) {
        j[kJsonKey_CallUUID] = t.callUUID();
        j[kJsonKey_CreatedAt] = t.createdAt();
        j[kJsonKey_SdpMLineIndex] = t.sdpMLineIndex();
        j[kJsonKey_SdpMid] = t.sdpMid();
        j[kJsonKey_Sdp] = t.sdp();
    }

    static virgil::voip::IceCandidate
    from_json(const json &j) {
        auto callUUID = j.at(kJsonKey_CallUUID).get<std::string>();
        auto createdAt = j.at(kJsonKey_CreatedAt).get<std::time_t>();
        auto sdpMLineIndex = j.at(kJsonKey_SdpMLineIndex).get<int>();
        auto sdpMid = j.at(kJsonKey_SdpMid).get<std::string>();
        auto sdp = j.at(kJsonKey_Sdp).get<std::string>();

        return {callUUID, sdpMLineIndex, sdpMid, sdp, createdAt};
    }
};
} // namespace nlohmann


using namespace virgil::voip;


CallSignalingMessage::CallSignalingMessage(std::string callUUID)
    : callUUID_(std::move(callUUID)), createdAt_(std::time(nullptr)) {
}

CallSignalingMessage::CallSignalingMessage(std::string callUUID, std::time_t createdAt)
    : callUUID_(std::move(callUUID)), createdAt_(createdAt) {
}

std::time_t
CallSignalingMessage::createdAt() const noexcept {
    return createdAt_;
}

std::string
CallSignalingMessage::callUUID() const noexcept {
    return callUUID_;
}

std::string
CallSignalingMessage::toJsonString(const CallSignalingMessage &callSignalingMessage) {
    nlohmann::json j;
    auto messageType = callSignalingMessage.type();
    switch (messageType) {
        case CallSignalingMessage::Type::callOffer:
            j[kJsonKey_Type] = kJsonValue_CallOffer;
            j[kJsonKey_Payload] = static_cast<const CallOffer &>(callSignalingMessage);
            break;

        case CallSignalingMessage::Type::callAnswer:
            j[kJsonKey_Type] = kJsonValue_CallAnswer;
            j[kJsonKey_Payload] = static_cast<const CallAnswer &>(callSignalingMessage);
            break;

        case CallSignalingMessage::Type::callReceived:
            j[kJsonKey_Type] = kJsonValue_CallReceived;
            j[kJsonKey_Payload] = static_cast<const CallReceived &>(callSignalingMessage);
            break;

        case CallSignalingMessage::Type::callRejected:
            j[kJsonKey_Type] = kJsonValue_CallRejected;
            j[kJsonKey_Payload] = static_cast<const CallRejected &>(callSignalingMessage);
            break;

        case CallSignalingMessage::Type::callEnded:
            j[kJsonKey_Type] = kJsonValue_CallEnded;
            j[kJsonKey_Payload] = static_cast<const CallEnded &>(callSignalingMessage);
            break;

        case CallSignalingMessage::Type::iceCandidate:
            j[kJsonKey_Type] = kJsonValue_IceCandidate;
            j[kJsonKey_Payload] = static_cast<const IceCandidate &>(callSignalingMessage);
            break;

        default:
            throw std::runtime_error("Unexpected message type");
    }

    return j.dump();
}

std::unique_ptr<CallSignalingMessage>
CallSignalingMessage::fromJsonString(const std::string &jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        auto messageType = j.at(kJsonKey_Type).get<std::string>();

        if (messageType == kJsonValue_CallOffer) {
            return std::make_unique<CallOffer>(j.at(kJsonKey_Payload).get<CallOffer>());

        } else if (messageType == kJsonValue_CallAnswer) {
            return std::make_unique<CallAnswer>(j.at(kJsonKey_Payload).get<CallAnswer>());

        } else if (messageType == kJsonValue_CallReceived) {
            return std::make_unique<CallReceived>(j.at(kJsonKey_Payload).get<CallReceived>());

        } else if (messageType == kJsonValue_CallRejected) {
            return std::make_unique<CallRejected>(j.at(kJsonKey_Payload).get<CallRejected>());

        } else if (messageType == kJsonValue_CallEnded) {
            return std::make_unique<CallEnded>(j.at(kJsonKey_Payload).get<CallEnded>());

        } else if (messageType == kJsonValue_IceCandidate) {
            return std::make_unique<IceCandidate>(j.at(kJsonKey_Payload).get<IceCandidate>());

        } else {
            throw CallException(CallError::FailedToParseCallSignalingMessage, "Unsupported message type.");
        }
    } catch (const nlohmann::detail::exception &e) {
        throw CallException(CallError::FailedToParseCallSignalingMessage, e.what());
    }
}
