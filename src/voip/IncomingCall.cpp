#include "voip/IncomingCall.h"

#include "CallConnectionFactory.h"
#include "CallAnswer.h"
#include "CallReceived.h"
#include "CallException.h"

#include "Observers.h"


using namespace virgil::voip;

IncomingCall::IncomingCall(const CallOffer &callOffer, std::string myName)
    : Call(callOffer.callUUID(), std::move(myName), callOffer.caller()), sdpString_(callOffer.sdp()) {
}


bool
IncomingCall::isOutgoing() const noexcept {
    return false;
}


void
IncomingCall::start() {

    try {
        CallConnectionFactory::sharedInstance().setupPeerConnection(*this);

    } catch (const CallException &callException) {
        this->end(callException.error());
        return;
    }

    auto observer = Observers::makeSetSessionDescriptionObserver(
            // On Success.
            [this]() {
                auto message = CallReceived(this->uuid());
                this->sendSignalingMessage(message);
                this->changePhase(CallPhase::started);
            },
            // On Failure.
            [this](webrtc::RTCError) {
                this->end(CallError::FailedToSetRemoteSessionDescription);
            });

    this->doPeerConnectionOp([this, observer](auto peerConnection) mutable {
        auto sessionDescription = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdpString_);
        if (!sessionDescription) {
            this->end(CallError::FailedToParseSessionDescription);
            return;
        }

        peerConnection->SetRemoteDescription(observer.release(), sessionDescription.release());
    });
}

void
IncomingCall::answer() {

    this->changePhase(CallPhase::accepted);

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
                            auto message = CallAnswer(this->uuid(), sdpString);
                            this->sendSignalingMessage(message);
                            this->changePhase(CallPhase::accepted);
                        },
                        [this](webrtc::RTCError) {
                            this->end(CallError::FailedToSetLocalSessionDescription);
                        });

                this->doPeerConnectionOp([observer, sdp](auto peerConnection) mutable {
                    peerConnection->SetLocalDescription(observer.release(), sdp);
                });
            },
            // On Failure.
            [this](webrtc::RTCError) {
                this->end(CallError::FailedToCreateCallAnswer);
            });


    auto callOptions = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions();
    callOptions.offer_to_receive_audio = 1;

    this->doPeerConnectionOp([observer, callOptions{std::move(callOptions)}](auto peerConnection) mutable {
        peerConnection->CreateAnswer(observer.release(), callOptions);
    });
}
