#ifndef VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_JSON_SERIALIZER_H_INCLUDED
#define VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_JSON_SERIALIZER_H_INCLUDED

#include "CallOffer.h"
#include "CallAnswer.h"
#include "IceCandidate.h"


namespace nlohmann {
//
// CallOffer.
//
template <>
struct adl_serializer<virgi::voip::CallOffer> {
    static virgi::voip::CallOffer
    from_json(const json &j) {
        return {j.get<int>()};
    }

    static void
    to_json(json &j, const virgi::voip::CallOffer &t) {
        j = t.i;
    }
};
} // namespace nlohmann

#endif /* VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_JSON_SERIALIZER_H_INCLUDED */
