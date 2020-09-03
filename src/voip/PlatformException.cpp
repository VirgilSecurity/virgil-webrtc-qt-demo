#include <voip/PlatformException.h>

#include <sstream>

using namespace virgil::voip;

// clang-format off
#define CASE_ENUM_VALUE_RETURN_STR(A) case A : return #A;
// clang-format on

static std::string
stringify(PlatformError callError) {
    switch (callError) {
        CASE_ENUM_VALUE_RETURN_STR(PlatformError::FailedJNI);
        CASE_ENUM_VALUE_RETURN_STR(PlatformError::FailedPlatformAudio);
        CASE_ENUM_VALUE_RETURN_STR(PlatformError::FailedPlatformCallManager);
        CASE_ENUM_VALUE_RETURN_STR(PlatformError::PlatformCallManager_ApplicationIsNotRegistered);
    }
}

static std::string
formatMessage(PlatformError callError, const std::string &details) {
    std::ostringstream ss;

    ss << stringify(callError);

    if (!details.empty()) {
        ss << ": " << details;
    }

    return ss.str();
}

PlatformException::PlatformException(PlatformError callError, const std::string &details)
    : std::runtime_error(formatMessage(callError, details)), error_(callError) {
}


PlatformError
PlatformException::error() const noexcept {
    return error_;
}
