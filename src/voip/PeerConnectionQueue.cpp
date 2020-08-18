#include "PeerConnectionQueue.h"

using namespace virgil::voip;

virgil::dispatch::dispatch_queue &
PeerConnectionQueue::sharedInstance() {
    static virgil::dispatch::dispatch_queue instance{"peer_connectionqueue", 1};
    return instance;
}
