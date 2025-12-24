#ifndef BANK_MSGQUEUE_H
#define BANK_MSGQUEUE_H
#include "./messages/Info.h"
#include "./messages/Order.h"
#include "./messages/PortfolioTrend.h"
#include "./messages/Stop.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <variant>

namespace bank::server {
template <typename T>
using Message =
    std::variant<messages::OrderRequest<T>, messages::InfoRequest<T>,
                 messages::PortfolioTrend<T>, messages::Stop>;
template <typename T> class MessageQueue {
public:
  MessageQueue(unsigned long maxSize) : maxSize(maxSize) {}

  void push(Message<T> &&msg);

  Message<T> pop();

private:
  std::queue<Message<T>> queue;
  unsigned long maxSize;
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
};
} // namespace bank::server
#endif // BANK_MSGQUEUE_H
