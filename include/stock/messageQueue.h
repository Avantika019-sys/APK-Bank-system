#ifndef BANK_MESSAGEQUEUE_H
#define BANK_MESSAGEQUEUE_H
#include <condition_variable>
#include <mutex>
#include <queue>

#include "visitor.h"

namespace stock {
class messageQueue {
public:
  messageQueue(unsigned long maxSize) : maxSize(maxSize) {}

  void push(variant msg);

  variant pop();

private:
  std::queue<variant> queue;
  unsigned long maxSize;
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
};
} // namespace stock
#endif // BANK_MESSAGEQUEUE_H
