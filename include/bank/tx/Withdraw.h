
#include <chrono>
#include <memory_resource>
#ifndef BANK_TXWITHDRAW_H
#define BANK_TXWITHDRAW_H
namespace tx {

struct Withdraw {
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::chrono::system_clock::time_point getCreatedAt() const {
    return createdAt_;
  }
  std::string toString() const {
    return "Transaction type: withdrawal\nAmount withdrawn: " +
           std::to_string(total);
  }
};
} // namespace tx
#endif // BANK_TXWITHDRAW_H
