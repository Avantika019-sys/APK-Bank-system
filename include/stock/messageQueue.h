//
// Created by ali on 11/13/25.
//

#ifndef BANK_MESSAGEQUEUE_H
#define BANK_MESSAGEQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

#include "visitor.h"


namespace stock {
    class messageQueue {
    public:
        messageQueue(unsigned long maxSize) : maxSize(maxSize) {
        }

        void push(variant msg);

        void pop(variant &msg);

    private:
        std::queue<variant> queue;
        unsigned long maxSize;
        std::mutex mtx;
        std::condition_variable cv_not_empty;
        std::condition_variable cv_not_full;
    };
}
#endif //BANK_MESSAGEQUEUE_H
