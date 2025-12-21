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

namespace bank::stock {
typedef std::variant<messages::Order, messages::InfoRequest,
                     messages::PortfolioTrend, messages::Stop>
    Message;
class MessageQueue {
public:
  MessageQueue(unsigned long maxSize) : maxSize(maxSize) {}

  void push(Message &&msg);

  Message pop();

private:
  std::queue<Message> queue;
  unsigned long maxSize;
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
};
} // namespace bank::stock
#endif // BANK_MSGQUEUE_H
