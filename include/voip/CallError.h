#ifndef VIRGIL_VOIP_CALL_ERROR_H_INCLUDED
#define VIRGIL_VOIP_CALL_ERROR_H_INCLUDED

namespace virgil {
namespace voip {

enum class CallError {
    FailedToConfigurePeerConnection,
    FailedToCreateCallOffer,
    FailedToSetLocalOffer,
    FailedToExportSessionDescription,
    FailedToParseSessionDescription,
    NoPeerConnection,
    UuidMismatch
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_ERROR_H_INCLUDED */
