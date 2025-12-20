#ifndef BANK_MSGQUEUE_H
#define BANK_MSGQUEUE_H
#include <condition_variable>
#include <mutex>
#include <queue>

#include "variant.h"

namespace bank::stock {
class MsgQueue {
public:
  MsgQueue(unsigned long maxSize) : maxSize(maxSize) {}

  void push(variant &&msg);

  variant pop();

private:
  std::queue<variant> queue;
  unsigned long maxSize;
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
};
} // namespace bank::stock
#endif // BANK_MSGQUEUE_H
