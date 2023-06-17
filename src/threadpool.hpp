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
#include "taskqueue.hpp"


namespace dreadpools {

class ThreadPool {
public:
    ThreadPool(const std::size_t count_threads);

    template <typename F, typename ... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

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
