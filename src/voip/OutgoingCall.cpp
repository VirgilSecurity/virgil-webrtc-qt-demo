#include "voip/OutgoingCall.h"

#include "CallConnectionFactory.h"
#include "CallOffer.h"
#include "CallException.h"

#include "Observers.h"


using namespace virgil::voip;

bool
OutgoingCall::isOutgoing() const noexcept {
    return true;
}


void
OutgoingCall::start() {

    try {
        CallConnectionFactory::sharedInstance().setupPeerConnection(*this);

    } catch (const CallException &callException) {
        this->die(callException.error());
        return;
    }

    auto observer = Observers::makeCreateSessionDescriptionObserver(
            // On Success.
            [this](webrtc::SessionDescriptionInterface *sdp) {
                std::string sdpString;

                if (!sdp->ToString(&sdpString)) {
                    this->end(CallError::FailedToExportSessionDescription);
                    return;
                }

                auto observer = Observers::makeSetSessionDescriptionObserver(
                        // On Success.
                        [this, sdpString = std::move(sdpString)]() {
                            auto message = CallOffer(this->uuid(), this->myName(), sdpString);
                            this->sendSignalingMessage(message);
                            this->changePhase(CallPhase::started);
                        },
                        // On Failure.
                        [this](webrtc::RTCError) {
                            this->end(CallError::FailedToSetLocalSessionDescription);
                        });

                this->doPeerConnectionOp([observer, sdp](auto peerConnection) mutable {
                    peerConnection->SetLocalDescription(observer.release(), sdp);
                });
            },
            // On Failure.
            [this](webrtc::RTCError) {
                this->end(CallError::FailedToCreateCallOffer);
            });

    auto callOptions = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions();
    callOptions.offer_to_receive_audio = 1;

    this->doPeerConnectionOp([observer, callOptions{std::move(callOptions)}](auto peerConnection) mutable {
        peerConnection->CreateOffer(observer.release(), callOptions);
    });
}


void
OutgoingCall::accept(const CallAnswer &callAnswer) {

    if (callAnswer.callUUID() != this->uuid()) {
        throw CallException(CallError::UuidMismatch, "Can not accept a call");
    }

    auto observer = Observers::makeSetSessionDescriptionObserver(
            [this] {
                this->changePhase(CallPhase::accepted);
            },
            [this](webrtc::RTCError) {
                this->end(CallError::FailedToSetRemoteSessionDescription);
            });

    this->doPeerConnectionOp([this, observer, sdpString = callAnswer.sdp()](auto peerConnection) mutable {
        auto sessionDescription = webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdpString);
        if (!sessionDescription) {
            this->end(CallError::FailedToParseSessionDescription);
            return;
        }

        peerConnection->SetRemoteDescription(observer.release(), sessionDescription.release());
    });
}
