#include <chrono>
#include <memory_resource>
#ifndef BANK_TXDEPOSIT_H
#define BANK_TXDEPOSIT_H
namespace tx {
struct Deposit {
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return " transaction type: deposit\nAmount depositted: " +
           std::to_string(total);
  }
};
} // namespace tx
#endif // BANK_TXDEPOSIT_H
