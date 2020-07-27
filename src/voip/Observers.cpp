#include "Observers.h"

#include <webrtc/rtc_base/ref_counted_object.h>


using namespace virgil::voip;

namespace {

// --------------------------------------------------------------------------
//  Implementation of webrtc::CreateSessionDescriptionObserver
// --------------------------------------------------------------------------
class CreateSessionDescriptionObserverWrapper : public rtc::RefCountedObject<webrtc::CreateSessionDescriptionObserver> {

public:
    CreateSessionDescriptionObserverWrapper(Observers::OnSuccessFunc onSuccess, Observers::OnFailureFunc onFailure)
        : m_onSuccess(std::move(onSuccess)), m_onFailure(std::move(onFailure)) {
    }

    virtual void
    OnSuccess(webrtc::SessionDescriptionInterface *desc) override {
        m_onSuccess(std::unique_ptr<webrtc::SessionDescriptionInterface>(desc));
    }

    virtual void
    OnFailure(webrtc::RTCError error) override {
        m_onFailure(error);
    }

private:
    Observers::OnSuccessFunc m_onSuccess;
    Observers::OnFailureFunc m_onFailure;
};

// --------------------------------------------------------------------------
//  Implementation of webrtc::SetSessionDescriptionObserver
// --------------------------------------------------------------------------
class SetSessionDescriptionObserverWrapper : public rtc::RefCountedObject<webrtc::SetSessionDescriptionObserver> {

public:
    SetSessionDescriptionObserverWrapper(Observers::OnSuccessVoidFunc onSuccess, Observers::OnFailureFunc onFailure)
        : m_onSuccess(std::move(onSuccess)), m_onFailure(std::move(onFailure)) {
    }

    virtual void
    OnSuccess() override {
        m_onSuccess();
    }

    virtual void
    OnFailure(webrtc::RTCError error) override {
        m_onFailure(error);
    }

private:
    Observers::OnSuccessVoidFunc m_onSuccess;
    Observers::OnFailureFunc m_onFailure;
};

} // namespace


rtc::scoped_refptr<webrtc::CreateSessionDescriptionObserver>
Observers::makeCreateSessionDescriptionObserver(OnSuccessFunc onSuccess, OnFailureFunc onFailure) {
    return new CreateSessionDescriptionObserverWrapper(std::move(onSuccess), std::move(onFailure));
}

rtc::scoped_refptr<webrtc::SetSessionDescriptionObserver>
Observers::makeSetSessionDescriptionObserver(OnSuccessVoidFunc onSuccess, OnFailureFunc onFailure) {
    return new SetSessionDescriptionObserverWrapper(std::move(onSuccess), std::move(onFailure));
}
