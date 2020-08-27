#ifndef VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED
#define VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED

#include "Call.h"
#include "IncomingCall.h"
#include "OutgoingCall.h"
#include "PlatformAudio.h"

#include "utils/quuid_unordered_map_spec.h"

#include <sigslot/signal.hpp>

#include <QObject>
#include <QString>
#include <QUuid>

#include <unordered_map>
#include <memory>
#include <string>


namespace virgil {
namespace voip {

class CallOffer;
class CallAnswer;
class IceCandidate;

class CallManager {
public:
    explicit CallManager(QString myId, std::unique_ptr<PlatformAudio> platformAudio = nullptr);

    const QString &
    myId() const noexcept;

    std::shared_ptr<OutgoingCall>
    createOutgoingCall(QString callee);

    std::shared_ptr<IncomingCall>
    createIncomingCall(const CallOffer &callOffer);

    std::shared_ptr<Call>
    findCall(const QUuid &uuid);

    std::shared_ptr<IncomingCall>
    findIncomingCall(const QUuid &uuid);

    std::shared_ptr<OutgoingCall>
    findOutgoingCall(const QUuid &uuid);

    void
    processCallAnswer(const CallAnswer &callAnswer);

    void
    processIceCandidate(const IceCandidate &iceCandidate);

    void
    setMicrophoneOn(bool on);

    void
    setVoiceOn(bool on);

    bool
    hasSpeaker() const;

    void
    setSpeakerOn(bool on);

    void
    setHoldOn(bool on);

    psigslot::signal<const Call &, CallPhase> callPhaseChanged;

    psigslot::signal<const Call &, CallConnectionState> callConnectionStateChanged;

    psigslot::signal<const Call &, CallError> callFailed;

    psigslot::signal<const Call &, const CallSignalingMessage &> createdMessageToSent;

private:
    void
    connectCall(std::shared_ptr<Call> call);

private:
    QString m_myId;
    std::unique_ptr<PlatformAudio> m_platformAudio;
    std::unordered_map<QUuid, std::shared_ptr<Call>> m_calls;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_MANAGER_H_INCLUDED */
