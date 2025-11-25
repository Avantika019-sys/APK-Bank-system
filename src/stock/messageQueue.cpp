#include "stock/messageQueue.h"
namespace stock {
void messageQueue::push(stockVariant msg) {

  std::unique_lock<std::mutex> lock(mtx);

  cv_not_full.wait(lock, [this] { return queue.size() < maxSize; });

  queue.push(std::move(msg));
  cv_not_empty.notify_one();
}

void messageQueue::pop(stockVariant &msg) {
  std::unique_lock<std::mutex> lock(mtx);

  cv_not_empty.wait(lock, [this] { return !queue.empty(); });

  msg = std::move(queue.front());
  queue.pop();
  cv_not_full.notify_one();
}
} // namespace stock
