#ifndef VIRGIL_VOIP_PLATFORM_EXCEPTION_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_EXCEPTION_H_INCLUDED

#include <stdexcept>
#include <string>

#include "PlatformError.h"

namespace virgil {
namespace voip {

class PlatformException : public std::runtime_error {
public:
    template <typename PointerType>
    static void
    throwIfNull(const PointerType &ptr, PlatformError callError, const std::string &details = "") {
        if (!ptr) {
            throw PlatformException(callError, details);
        }
    }

    static void
    throwIfFalse(bool expr, PlatformError callError, const std::string &details = "") {
        if (!expr) {
            throw PlatformException(callError, details);
        }
    }

    static void
    throwIfTrue(bool expr, PlatformError callError, const std::string &details = "") {
        if (expr) {
            throw PlatformException(callError, details);
        }
    }

    explicit PlatformException(PlatformError callError, const std::string &details = "");

    PlatformError
    error() const noexcept;

private:
    PlatformError error_;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_PLATFORM_EXCEPTION_H_INCLUDED */
