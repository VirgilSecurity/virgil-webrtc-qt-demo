#ifndef VIRGIL_VOIP_PLATFORM_ERROR_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_ERROR_H_INCLUDED

namespace virgil {
namespace voip {

enum class PlatformError {
    FailedJNI,
    FailedPlatformAudio,
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_PLATFORM_ERROR_H_INCLUDED */
