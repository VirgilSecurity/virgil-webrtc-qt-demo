#ifndef VIRGIL_VOIP_OUTGOING_CALL_H_INCLUDED
#define VIRGIL_VOIP_OUTGOING_CALL_H_INCLUDED


#include "voip/Call.h"
#include "voip/CallError.h"
#include "voip/CallAnswer.h"

namespace virgil {
namespace voip {

class OutgoingCall : public Call {
public:
    using Call::Call;

    virtual bool
    isOutgoing() const noexcept override;

    void
    start(OnSuccessFunc onSuccess, OnFailureFunc onFailure);

    void
    accept(const CallAnswer &callAnswer, OnSuccessFunc onSuccess, OnFailureFunc onFailure);

public Q_SLOTS:
    void
    remoteDidReceiveCall() noexcept;
};


} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_OUTGOING_CALL_H_INCLUDED */
