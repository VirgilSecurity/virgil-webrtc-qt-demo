#ifndef VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED
#define VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED

#include "voip/Call.h"

#include <webrtc/api/peer_connection_interface.h>
#include <webrtc/rtc_base/thread_checker.h>

namespace virgil {
namespace voip {

class CallConnectionFactory {
public:
    static CallConnectionFactory &
    sharedInstance();

    void
    setupPeerConnection(Call &call) const RTC_RUN_ON(m_thread_checker);

private:
    CallConnectionFactory();
    ~CallConnectionFactory();

private:
    rtc::ThreadChecker m_thread_checker;
    webrtc::Mutex m_pc_mutex;

    std::unique_ptr<rtc::Thread> m_network_thread RTC_GUARDED_BY(m_thread_checker);
    std::unique_ptr<rtc::Thread> m_worker_thread RTC_GUARDED_BY(m_thread_checker);
    std::unique_ptr<rtc::Thread> m_signaling_thread RTC_GUARDED_BY(m_thread_checker);

    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> m_peerConnectionFactory RTC_GUARDED_BY(m_thread_checker);
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_CALL_CONNECTION_FACTORY_H_INCLUDED */
