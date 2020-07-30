#ifndef VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED
#define VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED

#include <QObject>
#include <QJsonObject>

namespace virgil {
namespace voip {

/**
 * Base class for a call signaling messeges.
 */
class CallSignalingMessage : public QObject {
    Q_OBJECT

public:
    enum class Type {
        callOffer,
        callAnswer,
        callUpdate,
        iceCandidate,
    };
public:

    virtual QJsonObject
    toJson() const = 0;

    virtual Type
    type() const = 0;

    virtual QString
    toJsonString() const;

    virtual ~CallSignalingMessage() noexcept = default;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_SIGNALING_MESSAGE_H_INCLUDED */
