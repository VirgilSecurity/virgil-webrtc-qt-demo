#ifndef VIRGIL_VOIP_CALL_ERROR_H_INCLUDED
#define VIRGIL_VOIP_CALL_ERROR_H_INCLUDED

#include <QObject>

namespace virgil {
namespace voip {

enum class CallError {
    FailedToConfigureWebRTC,
    FailedToConfigurePeerConnection,
    FailedToCreateCallOffer,
    FailedToCreateCallAnswer,
    FailedToSetLocalSessionDescription,
    FailedToSetRemoteSessionDescription,
    FailedToExportSessionDescription,
    FailedToParseSessionDescription,
    FailedToParseCallOffer,
    FailedToParseCallAnswer,
    FailedToParseCallUpdate,
    FailedToParseIceCandidate,
    NoPeerConnection,
    UuidMismatch
};

} // namespace voip
} // namespace virgil

Q_DECLARE_METATYPE(virgil::voip::CallError)

#endif /* VIRGIL_VOIP_CALL_ERROR_H_INCLUDED */
