#ifndef VIRGIL_VOIP_PEER_CONNECTION_QUEUE_H_INCLUDED
#define VIRGIL_VOIP_PEER_CONNECTION_QUEUE_H_INCLUDED

#include "queue/dispatch_queue.h"

namespace virgil {
namespace voip {

class PeerConnectionQueue {
public:
    static virgil::dispatch::dispatch_queue &
    sharedInstance();

private:
    PeerConnectionQueue() = delete;

private:
    virgil::dispatch::dispatch_queue m_queue;
};

} // namespace voip
} // namespace virgil

#endif /* VIRGIL_VOIP_PEER_CONNECTION_QUEUE_H_INCLUDED */
