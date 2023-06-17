#include "threadpool.hpp"


namespace dreadpools {

ThreadPool::ThreadPool(const std::size_t count_threads) : threads(count_threads) {
    for (auto& t : threads) {
        t = std::jthread(ThreadWorker(*this));
    }
}

ThreadPool::~ThreadPool() {
    stop_source.request_stop();
    _cv.notify_all();
}


void ThreadWorker::operator()() {
    while (!stop_token.stop_requested()) {
        std::function<void()> task;
        bool has_task = false;
        while (!has_task && !stop_token.stop_requested()) {
            std::unique_lock lock(pool._cv_m);
            if (pool._tasks.empty()) {
                pool._cv.wait(lock);
            }
            has_task = (pool._tasks.dequeue(task));
        }
        if (!stop_token.stop_requested()) {
            task();  // perform task
        }
    }
}

}
