#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <cstddef>
#include <thread>
#include <vector>
#include <iostream>
#include <stop_token>
#include <future>
#include <type_traits>
#include <algorithm>
#include "taskqueue.hpp"


namespace dreadpools {

class ThreadPool {
public:
    // std::thread::hardware_concurrency() can return 0
    ThreadPool(
        const unsigned int count_threads = std::max(1u, std::thread::hardware_concurrency()) - 1u
    ) : threads(count_threads) {};

    void start();

    void join();

    template <typename F, typename ... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        typedef decltype(f(args...)) rt;
        std::function<rt()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<rt()>>(func);

        std::function<void()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
        };

        _tasks.enqueue(wrapper_func);
        _cv.notify_one();
        return task_ptr->get_future();
    }

    ~ThreadPool();

private:
    std::vector<std::jthread> threads;
    std::condition_variable _cv{};
    std::mutex _cv_m{};
    TaskQueue<std::function<void()>> _tasks{};
    std::stop_source _stop_source;
    friend class ThreadWorker;
};


class ThreadWorker {
public:
    explicit ThreadWorker(ThreadPool& p) :
        _pool(p),
        _stop_token(p._stop_source.get_token()
    ) {}
    void operator()();

private:
    ThreadPool& _pool;
    std::stop_token _stop_token;
};

}
