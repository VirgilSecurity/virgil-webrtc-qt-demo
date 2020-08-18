#ifndef VIRGIL_DISPATCH_QUEUE_H_INCLUDED
#define VIRGIL_DISPATCH_QUEUE_H_INCLUDED

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace virgil {
namespace dispatch {

/**
 * Implementation taken from https://github.com/embeddedartistry/embedded-resources
 * Author: https://github.com/phillipjohnston
 * License: CC0
 */

class dispatch_queue {
    typedef std::function<void(void)> fp_t;

public:
    dispatch_queue(std::string name, size_t thread_cnt = 1);
    ~dispatch_queue();

    void
    dispatch(const fp_t &op);
    void
    dispatch(fp_t &&op);

    dispatch_queue(const dispatch_queue &rhs) = delete;

    dispatch_queue &
    operator=(const dispatch_queue &rhs) = delete;

    dispatch_queue(dispatch_queue &&rhs) = delete;

    dispatch_queue &
    operator=(dispatch_queue &&rhs) = delete;

private:
    std::string name_;
    std::mutex lock_;
    std::vector<std::thread> threads_;
    std::queue<fp_t> q_;
    std::condition_variable cv_;
    bool quit_ = false;

    void
    dispatch_thread_handler(void);
};

} // namespace dispatch
} // namespace virgil


#endif /* VIRGIL_DISPATCH_QUEUE_H_INCLUDED */
