#include <voip/CallException.h>

#include <sstream>

using namespace virgil::voip;

// clang-format off
#define CASE_ENUM_VALUE_RETURN_STR(A) case A : return #A;
// clang-format on

static std::string
stringify(CallError callError) {
    switch (callError) {
        CASE_ENUM_VALUE_RETURN_STR(CallError::FailedToConfigurePeerConnection);
        CASE_ENUM_VALUE_RETURN_STR(CallError::FailedToCreateCallOffer)
    }
}

static std::string
formatMessage(CallError callError, const std::string &details) {
    std::ostringstream ss;

    ss << stringify(callError);

    if (!details.empty()) {
        ss << ": " << details;
    }

    return ss.str();
}

CallException::CallException(CallError callError, const std::string &details)
    : std::runtime_error(formatMessage(callError, details)), m_callError(callError) {
}


CallError CallException::error() const noexcept {
    return m_callError;
}
