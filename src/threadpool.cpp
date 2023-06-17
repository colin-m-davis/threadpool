#include "threadpool.hpp"

namespace dreadpools {

void ThreadWorker::operator()() {
    while (pool.is_active) {
        std::unique_lock lock(pool._cv_m);
        if (!pool._tasks.empty()) {
            std::function<void()> task;
            pool._tasks.dequeue(task);
            //  lock.unlock();
        }
        pool._cv.wait(lock, [this]() {
            return !pool.is_active || !pool._tasks.empty();
        });
    }
}

}
