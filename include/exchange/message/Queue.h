#include "exchange/trait.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>

#ifndef EXCHANGE_MESSAGE_QUEUE_H
#define EXCHANGE_MESSAGE_QUEUE_H
namespace exchange::message {
template <typename T> using Message = exchange::trait::MessageQueue<T>::Variant;
template <typename T> class Queue {
public:
  Queue(unsigned long maxSize) : maxSize(maxSize) {}

  void push(message::Message<T> &&msg) {
    std::unique_lock<std::mutex> lock(mtx);

    cv_not_full.wait(lock, [this] { return queue.size() < maxSize; });

    queue.push(std::move(msg));
    cv_not_empty.notify_one();
  }

  message::Message<T> pop() {
    std::unique_lock<std::mutex> lock(mtx);

    cv_not_empty.wait(lock, [this] { return !queue.empty(); });

    auto msg = std::move(queue.front());
    queue.pop();
    cv_not_full.notify_one();
    return msg;
  }

private:
  std::queue<message::Message<T>> queue;
  unsigned long maxSize;
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_QUEUE_H
