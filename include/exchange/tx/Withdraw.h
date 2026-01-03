
#include "exchange/currency/DKK.h"
#include <chrono>
#include <memory_resource>
#ifndef EXCHANGE_TX_WITHDRAW_H
#define EXCHANGE_TX_WITHDRAW_H
namespace exchange::tx {
struct Withdraw {
  currency::DKK total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::chrono::system_clock::time_point getCreatedAt() const {
    return createdAt_;
  }
  std::string toString() const {
    return "Transaction type: withdrawal\nAmount withdrawn: " +
           total.toString();
  }
};
} // namespace exchange::tx
#endif // EXCHANGE_TX_WITHDRAW_H
