#pragma once

#include "concurrentqueue.h"
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class MessageQueue {
private:
    moodycamel::ConcurrentQueue<T> queue;
    std::mutex mtx;
    std::condition_variable conditionVariable;
    bool shutdown = false;
public:
    void enqueue(const T& item) {
        queue.enqueue(item);
        {
            // 只在锁内进行通知
            std::lock_guard<std::mutex> lock(mtx);
            conditionVariable.notify_one();
        }
    }
    std::optional<T> dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        conditionVariable.wait(
                lock, [this] {
                    return queue.size_approx() > 0 || shutdown;
                }
        );
        T item;
        if(queue.try_dequeue(item)){
            return item;
        }
        if(shutdown){
            return {};
        }
        return {};
    }
    void shutdownQueue() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            shutdown = true;
        }
        conditionVariable.notify_all();
    }
};