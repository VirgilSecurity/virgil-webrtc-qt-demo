#ifndef VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED
#define VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED

#include "voip/Call.h"

#include <webrtc/api/peer_connection_interface.h>

namespace virgil {
namespace voip {

class CallConnectionFactory {
public:
    static CallConnectionFactory &
    sharedInstance();

    void
    setupPeerConnection(Call &call) const;

private:
    CallConnectionFactory();

private:
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> m_peerConnectionFactory;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED */
