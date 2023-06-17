#include "threadpool.hpp"


namespace dreadpools {

void ThreadWorker::operator()() {
    // <stop_token>
    while (pool.is_active) {
        std::function<void()> task;
        bool has_task = false;
        while (!has_task) {
            std::unique_lock lock(pool._cv_m);
            if (pool._tasks.empty()) {
                pool._cv.wait(lock);
            }
            has_task = (pool._tasks.dequeue(task));
        }
        task();  // perform task
    }
}

}
