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

    try {
        CallConnectionFactory::sharedInstance().setupPeerConnection(*this);

    } catch (const CallException &callException) {
        onFailure(callException.error());
        return;
    }

    auto onCreateOfferSuccess = [this, onSuccess, onFailure](std::unique_ptr<webrtc::SessionDescriptionInterface> sdp) {
        std::string sdpString;

        if (!sdp->ToString(&sdpString)) {
            onFailure(CallError::FailedToExportSessionDescription);
            return;
        }

        auto onSetLocalDescriptionSuccess = [this, onSuccess, onFailure, sdpString = std::move(sdpString)]() {
            auto message = new CallOffer(this->uuid(),
                    QDateTime::currentDateTime(),
                    this->myName(),
                    QString::fromStdString(sdpString));

            Q_EMIT createdSignalingMessage(message);
            onSuccess();
        };

        auto onSetLocalDescriptionFailure = [onFailure](webrtc::RTCError error) {
            qDebug() << "Failed to set an offer session description as local session description: " << error.message();
            onFailure(CallError::FailedToSetLocalSessionDescription);
        };

        auto observer = Observers::makeSetSessionDescriptionObserver(std::move(onSetLocalDescriptionSuccess),
                std::move(onSetLocalDescriptionFailure));

        this->peerConnection()->SetLocalDescription(observer.release(), sdp.release());
    };

    auto onCreateOfferFailure = [=](webrtc::RTCError error) {
        qDebug() << "Can not create a call offer: " << error.message();
        onFailure(CallError::FailedToCreateCallOffer);
    };

    auto observer = Observers::makeCreateSessionDescriptionObserver(std::move(onCreateOfferSuccess),
            std::move(onCreateOfferFailure));

    auto callOptions = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions();

    callOptions.offer_to_receive_audio = 1;

    this->peerConnection()->CreateOffer(observer.release(), callOptions);
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
        qDebug() << "Failed to set an answer session description as remote session description: " << error.message();
        onFailure(CallError::FailedToSetRemoteSessionDescription);
    };

    auto observer = Observers::makeSetSessionDescriptionObserver(std::move(onSuccess),
            std::move(onSetRemoteDescriptionFailure));

    this->peerConnection()->SetRemoteDescription(observer.release(), sessionDescription.release());
}
