#include "threadpool.hpp"


namespace dreadpools {

ThreadPool::ThreadPool(const std::size_t count_threads) : threads(count_threads) {}

void ThreadPool::start() {
    for (auto& t : threads) {
        t = std::jthread(ThreadWorker(*this));
    }
}

ThreadPool::~ThreadPool() {
    _stop_source.request_stop();
    _cv.notify_all();
}


void ThreadWorker::operator()() {
    while (!_stop_token.stop_requested()) {
        std::function<void()> task;
        bool has_task = false;
        while (!has_task && !_stop_token.stop_requested()) {
            std::unique_lock lock(_pool._cv_m);
            if (_pool._tasks.empty()) {
                _pool._cv.wait(lock);
            }
            has_task = _pool._tasks.dequeue(task);
        }
        if (!_stop_token.stop_requested()) {
            task();  // perform task
        }
    }
}

}
