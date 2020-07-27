#ifndef VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED
#define VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED

#include <stdexcept>
#include <string>

#include <webrtc/api/scoped_refptr.h>

#include "CallError.h"

namespace virgil {
namespace voip {

class CallException : public std::runtime_error {
public:
    template <typename T>
    static void
    throwIfNull(rtc::scoped_refptr<T> refptr, CallError callError, const std::string &details = "") {
        if (refptr.get() == nullptr) {
            throw CallException(callError, details);
        }
    }

    explicit CallException(CallError callError, const std::string &details = "");

    CallError error() const noexcept;

private:
    CallError m_callError;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_EXCEPTION_H_INCLUDED */
