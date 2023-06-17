#include "threadpool.hpp"


namespace dreadpools {

ThreadPool::ThreadPool(const std::size_t count_threads) : threads(count_threads) {
    for (auto& t : threads) {
        t = std::thread(std::thread(ThreadWorker(*this)));
    }
}


void ThreadWorker::operator()() {
    std::cout << "operator()\n";
    // <stop_token>
    while (true) {
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
