#ifndef VIRGIL_VOIP_CALL_ERROR_H_INCLUDED
#define VIRGIL_VOIP_CALL_ERROR_H_INCLUDED

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
    FailedToParseCallSignalingMessage,
    FailedToParseIceCandidate,
    FailedToSendCallSignalingMessage,
    FailedToConnect,
    NoPeerConnection,
    UuidMismatch
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_ERROR_H_INCLUDED */
