#ifndef VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED
#define VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED


#include "Call.h"
#include "CallError.h"
#include "CallOffer.h"

namespace virgil {
namespace voip {

class IncomingCall : public Call {
public:
    IncomingCall(const CallOffer &callOffer, std::string myName);

    virtual bool
    isOutgoing() const noexcept override;

    void
    start();

    void
    answer();

private:
    std::string sdpString_;
};


} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED */
