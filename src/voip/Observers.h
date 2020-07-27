#ifndef VIRGIL_VOIP_CREATE_OBSERVERS_H_INCLUDED
#define VIRGIL_VOIP_CREATE_OBSERVERS_H_INCLUDED

#include <webrtc/api/jsep.h>
#include <webrtc/api/scoped_refptr.h>

#include <functional>
#include <memory>

namespace virgil {
namespace voip {


class Observers {
public:
    using OnSuccessFunc = std::function<void(std::unique_ptr<webrtc::SessionDescriptionInterface> desc)>;
    using OnFailureFunc = std::function<void(webrtc::RTCError error)>;

    using OnSuccessVoidFunc = std::function<void()>;

    static rtc::scoped_refptr<webrtc::CreateSessionDescriptionObserver>
    makeCreateSessionDescriptionObserver(OnSuccessFunc onSuccess, OnFailureFunc onFailure);

    static rtc::scoped_refptr<webrtc::SetSessionDescriptionObserver>
    makeSetSessionDescriptionObserver(OnSuccessVoidFunc onSuccess, OnFailureFunc onFailure);

    Observers() = delete;
};


} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CREATE_OBSERVERS_H_INCLUDED */
