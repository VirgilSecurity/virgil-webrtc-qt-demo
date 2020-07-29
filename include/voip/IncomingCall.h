#ifndef VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED
#define VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED


#include "voip/Call.h"
#include "voip/CallError.h"
#include "voip/CallOffer.h"

namespace virgil {
namespace voip {

class IncomingCall : public Call {
    Q_OBJECT

public:
    IncomingCall(const CallOffer& callOffer, QString myName);

    virtual bool
    isOutgoing() const noexcept override;

    void
    start(OnSuccessFunc onSuccess, OnFailureFunc onFailure);

    void
    answer(OnSuccessFunc onSuccess, OnFailureFunc onFailure);

private:
    QString m_sdpString;
};


} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_INCOMING_CALL_H_INCLUDED */
