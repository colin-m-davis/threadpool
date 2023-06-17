#pragma once

#include <queue>
#include <mutex>
#include <utility>

namespace dreadpools {

template <typename T>
class MessageQueue {
public:
    [[nodiscard]] bool empty() const {
        lock_t lock(_mutex);
        return _queue.empty();
    }

    template <typename ... Args>
    void enqueue(Args&& ... args) {
        lock_t lock(_mutex);
        _queue.emplace(std::forward<T>(args)...);
    }
    
    // caller is responsible for providing out param
    bool dequeue(T& out) {
        lock_t lock(_mutex);
        if (_queue.empty()) {
            return false;
        }
        out = std::move(_queue.front());
        _queue.pop();
        return true;
    }

private:
    std::queue<T> _queue{};
    std::mutex _mutex{};
    typedef std::lock_guard<std::mutex> lock_t;
};

}