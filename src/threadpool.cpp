#include "threadpool.hpp"


namespace dreadpools {

void ThreadPool::start() {
    for (auto& t : threads) {
        t = std::jthread(ThreadWorker(*this));
    }
}

// assures all tasks are completed before joining threads
void ThreadPool::join() {
    // adds a dummy task to end of queue
    auto fut = submit([]{});
    fut.get();
    // all tasks that were in the queue when join() was called have been picked up by a thread
    {
        std::unique_lock lock(_cv_m);
        _stop_source.request_stop();
        _cv.notify_all();
    }
    // wait for threads to complete their tasks as necessary
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
