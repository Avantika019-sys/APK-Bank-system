#include "exchange/currency/DKK.h"
#include <chrono>
#include <memory_resource>
#ifndef EXCHANGE_TX_DEPOSIT_H
#define EXCHANGE_TX_DEPOSIT_H
namespace exchange::tx {
struct Deposit {
  currency::DKK total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: deposit\nAmount depositted: " + total.toString();
  }
};
} // namespace exchange::tx
#endif // EXCHANGE_TX_DEPOSIT_H
