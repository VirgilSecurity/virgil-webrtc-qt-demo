#ifndef VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED
#define VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED

#include <stdexcept>
#include <string>

#include "CallError.h"

namespace virgil {
namespace voip {

class CallException : public std::runtime_error {
public:
    template <typename SmartPointerType>
    static void
    throwIfNull(const SmartPointerType &ptr, CallError callError, const std::string &details = "") {
        if (ptr.get() == nullptr) {
            throw CallException(callError, details);
        }
    }

    static void
    throwIfFalse(bool expr, CallError callError, const std::string &details = "") {
        if (!expr) {
            throw CallException(callError, details);
        }
    }

    explicit CallException(CallError callError, const std::string &details = "");

    CallError
    error() const noexcept;

private:
    CallError m_callError;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED */
