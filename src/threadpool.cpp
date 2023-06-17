#include "threadpool.hpp"


namespace dreadpools {

ThreadPool::ThreadPool(const std::size_t count_threads) : threads(count_threads) {}

void ThreadPool::start() {
    for (auto& t : threads) {
        t = std::jthread(ThreadWorker(*this));
    }
}

// assures all tasks are completed before joining threads by submitting an empty "shutdown" task
void ThreadPool::join() {
    auto fut = submit([]{});
    fut.get();
    {
        std::unique_lock lock(_cv_m);
        _stop_source.request_stop();
        _cv.notify_all();
    }
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
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
        {
            std::unique_lock lock(_pool._cv_m);
            while (_pool._tasks.empty() && !_stop_token.stop_requested()) {
                _pool._cv.wait(lock);
            }
            has_task = _pool._tasks.dequeue(task);
        }
        if (has_task) {
            task();  // perform task
        }
    }
}

}
