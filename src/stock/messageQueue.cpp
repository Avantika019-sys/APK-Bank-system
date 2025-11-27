#include "stock/messageQueue.h"
#include "stock/visitor.h"
namespace stock {
void messageQueue::push(variant msg) {

  std::unique_lock<std::mutex> lock(mtx);

  cv_not_full.wait(lock, [this] { return queue.size() < maxSize; });

  queue.push(std::move(msg));
  cv_not_empty.notify_one();
}

variant messageQueue::pop() {
  std::unique_lock<std::mutex> lock(mtx);

  cv_not_empty.wait(lock, [this] { return !queue.empty(); });

  auto msg = std::move(queue.front());
  queue.pop();
  cv_not_full.notify_one();
  return msg;
}
} // namespace stock
