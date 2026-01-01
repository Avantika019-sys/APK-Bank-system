#include <chrono>
#include <memory_resource>
#ifndef EXCHANGE_TX_DEPOSIT_H
#define EXCHANGE_TX_DEPOSIT_H
namespace exchange::tx {
struct Deposit {
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: deposit\nAmount depositted: " +
           std::to_string(total);
  }
};
} // namespace exchange::tx
#endif // EXCHANGE_TX_DEPOSIT_H
