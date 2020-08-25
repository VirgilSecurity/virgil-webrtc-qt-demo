#ifndef VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED
#define VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED

#include "voip/Call.h"

#include <webrtc/api/peer_connection_interface.h>
#include <webrtc/rtc_base/critical_section.h>
#include <webrtc/rtc_base/thread_annotations.h>
#include <webrtc/modules/audio_device/include/audio_device.h>

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
    ~CallConnectionFactory() noexcept = default;

private:
    rtc::CriticalSection factoryMutex_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory_ RTC_GUARDED_BY(factoryMutex_);
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED */
