#include "voip/OutgoingCall.h"

#include <QDebug>

#include "voip/CallConnectionFactory.h"
#include "voip/CallOffer.h"
#include "voip/CallException.h"

#include "Observers.h"


using namespace virgil::voip;

bool
OutgoingCall::isOutgoing() const noexcept {
    return true;
}


void
OutgoingCall::start(OnSuccessFunc onSuccess, OnFailureFunc onFailure) {
    CallConnectionFactory::sharedInstance().setupPeerConnection(*this);

    auto onCreateOfferSuccess = [this, onSuccess, onFailure](std::unique_ptr<webrtc::SessionDescriptionInterface> sdp) {
        std::string sdpString;

        if (!sdp->ToString(&sdpString)) {
            onFailure(CallError::FailedToExportSessionDescription);
            return;
        }

        auto onSetLocalDescriptionSuccess = [this, onSuccess, onFailure, sdpString = std::move(sdpString)]() {
            auto message = CallOffer(
                    this->uuid(), QDateTime::currentDateTime(), this->myName(), QString::fromStdString(sdpString));

            Q_EMIT createdSignalingMessage(message);
            onSuccess();
        };

        auto onSetLocalDescriptionFailure = [onFailure](webrtc::RTCError error) {
            qDebug() << "Can not set local offer: " << error.message();
            onFailure(CallError::FailedToSetLocalOffer);
        };

        auto observer = Observers::makeSetSessionDescriptionObserver(
                std::move(onSetLocalDescriptionSuccess), std::move(onSetLocalDescriptionFailure));

        this->peerConnection()->SetLocalDescription(observer.release(), sdp.release());
    };

    auto onCreateOfferFailure = [=](webrtc::RTCError error) {
        qDebug() << "Can not create call offer: " << error.message();
        onFailure(CallError::FailedToCreateCallOffer);
    };

    auto observer = Observers::makeCreateSessionDescriptionObserver(
            std::move(onCreateOfferSuccess), std::move(onCreateOfferFailure));

    this->peerConnection()->CreateOffer(observer.release(), webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}


void
OutgoingCall::accept(const CallAnswer &callAnswer, OnSuccessFunc onSuccess, OnFailureFunc onFailure) {

    if (callAnswer.callUUID() != this->uuid()) {
        throw CallException(CallError::UuidMismatch, "Can not accept a call");
    }

    this->changePhase(CallPhase::accepted);

    auto sdpString = callAnswer.sdp();

    auto sessionDescription = webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdpString.toStdString());
    if (!sessionDescription) {
        qDebug() << "Can not accept a call (failed to parse session description)";
        onFailure(CallError::FailedToParseSessionDescription);
        return;
    }

    auto onSetRemoteDescriptionFailure = [onFailure](webrtc::RTCError error) {
        qDebug() << "Can not set local offer: " << error.message();
        onFailure(CallError::FailedToSetLocalOffer);
    };

    auto observer = Observers::makeSetSessionDescriptionObserver(
            std::move(onSuccess), std::move(onSetRemoteDescriptionFailure));

    this->peerConnection()->SetRemoteDescription(observer.release(), sessionDescription.release());
}
