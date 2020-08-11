#include "voip/IncomingCall.h"

#include <QDebug>

#include "voip/CallConnectionFactory.h"
#include "voip/CallAnswer.h"
#include "voip/CallException.h"

#include "Observers.h"


using namespace virgil::voip;

IncomingCall::IncomingCall(const CallOffer &callOffer, QString myName)
    : Call(callOffer.callUUID(), std::move(myName), callOffer.caller()), m_sdpString(callOffer.sdp()) {
}


bool
IncomingCall::isOutgoing() const noexcept {
    return false;
}


void
IncomingCall::start(OnSuccessFunc onSuccess, OnFailureFunc onFailure) {

    try {
        CallConnectionFactory::sharedInstance().setupPeerConnection(*this);

    } catch (const CallException &callException) {
        onFailure(callException.error());
        return;
    }

    this->changePhase(CallPhase::ringing);

    auto sessionDescription = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, m_sdpString.toStdString());
    if (!sessionDescription) {
        qDebug() << "Can not start an incoming call (failed to parse session description)";
        onFailure(CallError::FailedToParseSessionDescription);
        return;
    }

    auto onSetRemoteDescriptionFailure = [onFailure](webrtc::RTCError error) {
        qDebug() << "Failed to set an offer session description as remote session description: " << error.message();
        onFailure(CallError::FailedToSetRemoteSessionDescription);
    };

    auto observer = Observers::makeSetSessionDescriptionObserver(std::move(onSuccess),
            std::move(onSetRemoteDescriptionFailure));

    rtc::CritScope lock(&m_peerConnectionMutex);
    this->peerConnection()->SetRemoteDescription(observer.release(), sessionDescription.release());
}


void
IncomingCall::answer(OnSuccessFunc onSuccess, OnFailureFunc onFailure) {

    this->changePhase(CallPhase::accepted);

    auto onCreateAnswerSuccess = [this, onSuccess, onFailure](
                                         std::unique_ptr<webrtc::SessionDescriptionInterface> sdp) {
        std::string sdpString;

        if (!sdp->ToString(&sdpString)) {
            onFailure(CallError::FailedToExportSessionDescription);
            return;
        }

        auto onSetLocalDescriptionSuccess = [this, onSuccess, onFailure, sdpString = std::move(sdpString)]() {
            auto message = CallAnswer(this->uuid(), QString::fromStdString(sdpString));

            createdSignalingMessage(message);

            onSuccess();
        };

        auto onSetLocalDescriptionFailure = [onFailure](webrtc::RTCError error) {
            qDebug() << "Failed to set an answer session description as local session description: " << error.message();
            onFailure(CallError::FailedToSetLocalSessionDescription);
        };

        auto observer = Observers::makeSetSessionDescriptionObserver(std::move(onSetLocalDescriptionSuccess),
                std::move(onSetLocalDescriptionFailure));

        rtc::CritScope lock(&m_peerConnectionMutex);
        this->peerConnection()->SetLocalDescription(observer.release(), sdp.release());
    };

    auto onCreateAnswerFailure = [=](webrtc::RTCError error) {
        qDebug() << "Can not create a call offer: " << error.message();
        onFailure(CallError::FailedToCreateCallAnswer);
    };

    auto observer = Observers::makeCreateSessionDescriptionObserver(std::move(onCreateAnswerSuccess),
            std::move(onCreateAnswerFailure));

    rtc::CritScope lock(&m_peerConnectionMutex);
    this->peerConnection()->CreateAnswer(observer.release(), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}
