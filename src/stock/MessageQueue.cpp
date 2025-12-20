#include "stock/MessageQueue.h"
namespace bank::stock {
void MessageQueue::push(Message &&msg) {

  std::unique_lock<std::mutex> lock(mtx);

  cv_not_full.wait(lock, [this] { return queue.size() < maxSize; });

  queue.push(std::move(msg));
  cv_not_empty.notify_one();
}

Message MessageQueue::pop() {
  std::unique_lock<std::mutex> lock(mtx);

  cv_not_empty.wait(lock, [this] { return !queue.empty(); });

  auto msg = std::move(queue.front());
  queue.pop();
  cv_not_full.notify_one();
  return msg;
}
} // namespace bank::stock
