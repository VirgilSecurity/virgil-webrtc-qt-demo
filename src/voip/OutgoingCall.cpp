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

    auto onCreateOfferSuccess = [this, onSuccess, onFailure](webrtc::SessionDescriptionInterface *sdp) {
        std::string sdpString;

        if (!sdp->ToString(&sdpString)) {
            onFailure(CallError::FailedToExportSessionDescription);
            return;
        }

        auto onSetLocalDescriptionSuccess = [this, onSuccess, onFailure, sdpString = std::move(sdpString)]() {
            auto message = CallOffer(
                    this->uuid(),
                    QDateTime::currentDateTime(),
                    this->myName(),
                    QString::fromStdString(sdpString));

            createdSignalingMessage(message);
            onSuccess();
        };

        auto onSetLocalDescriptionFailure = [onFailure](webrtc::RTCError error) {
            qDebug() << "Failed to set an offer session description as local session description: " << error.message();
            onFailure(CallError::FailedToSetLocalSessionDescription);
        };

        auto observer = Observers::makeSetSessionDescriptionObserver(
                std::move(onSetLocalDescriptionSuccess),
                std::move(onSetLocalDescriptionFailure));

        this->doPeerConnectionOp([observer, sdp](auto peerConnection) mutable {
            peerConnection->SetLocalDescription(observer.release(), sdp);
        });
    };

    auto onCreateOfferFailure = [=](webrtc::RTCError error) {
        qDebug() << "Can not create a call offer: " << error.message();
        onFailure(CallError::FailedToCreateCallOffer);
    };

    auto observer = Observers::makeCreateSessionDescriptionObserver(
            std::move(onCreateOfferSuccess),
            std::move(onCreateOfferFailure));

    auto callOptions = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions();
    callOptions.offer_to_receive_audio = 1;

    this->doPeerConnectionOp([observer, callOptions{std::move(callOptions)}](auto peerConnection) mutable {
        peerConnection->CreateOffer(observer.release(), callOptions);
    });
}


void
OutgoingCall::accept(const CallAnswer &callAnswer, OnSuccessFunc onSuccess, OnFailureFunc onFailure) {

    if (callAnswer.callUUID() != this->uuid()) {
        throw CallException(CallError::UuidMismatch, "Can not accept a call");
    }

    this->changePhase(CallPhase::accepted);

    auto onSetRemoteDescriptionFailure = [onFailure](webrtc::RTCError error) {
        qDebug() << "Failed to set an answer session description as remote session description: " << error.message();
        onFailure(CallError::FailedToSetRemoteSessionDescription);
    };

    auto observer = Observers::makeSetSessionDescriptionObserver(
            std::move(onSuccess),
            std::move(onSetRemoteDescriptionFailure));

    this->doPeerConnectionOp(
            [onFailure, observer, sdpString{callAnswer.sdp().toStdString()}](auto peerConnection) mutable {
                auto sessionDescription = webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdpString);
                if (!sessionDescription) {
                    qDebug() << "Can not accept a call (failed to parse session description)";
                    onFailure(CallError::FailedToParseSessionDescription);
                    return;
                }

                peerConnection->SetRemoteDescription(observer.release(), sessionDescription.release());
            });
}
