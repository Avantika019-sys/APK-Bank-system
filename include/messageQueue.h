//
// Created by ali on 11/13/25.
//

#ifndef BANK_MESSAGEQUEUE_H
#define BANK_MESSAGEQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class messageQueue {
public:
    messageQueue(unsigned long maxSize) : maxSize(maxSize) {
    }

    void push(T msg) {
        std::unique_lock<std::mutex> lock(mtx);

        cv_not_full.wait(lock, [this]{
            return queue.size() < maxSize;
        });

        queue.push(std::move(msg));
        cv_not_empty.notify_one();
    }

    void pop(T& msg) {
        std::unique_lock<std::mutex> lock(mtx);

        cv_not_empty.wait(lock, [this]{
            return !queue.empty();
        });

        msg = std::move(queue.front());
        queue.pop();
        cv_not_full.notify_one();
    }

private:
    std::queue<T> queue;
    unsigned long maxSize;
    std::mutex mtx;
    std::condition_variable cv_not_empty;
    std::condition_variable cv_not_full;
};
#endif //BANK_MESSAGEQUEUE_H