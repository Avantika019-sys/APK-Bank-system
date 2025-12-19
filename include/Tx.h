#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include "TxDetails.h"
#include <chrono>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

using namespace std;

class Tx {

public:
  Tx(details d, std::pmr::memory_resource *memRes);
  const details &getDetails() const;
  std::chrono::system_clock::time_point getCreatedAt() const;

private:
  std::pmr::memory_resource *memRes_;
  std::pmr::vector<std::pmr::string> details_;
  std::chrono::system_clock::time_point createdAt_;
  std::string from_account;
  std::string to_account;
  details details_;
  // std::string transaction_id_;
};
std::ostream &operator<<(std::ostream &os, const Tx &t);
// std::ostream &operator<<(std::ostream &os, const stockTx &t);
#endif // BANK_TRANSACTION_H
// using TransactionData =
//     std::variant<DepositTransaction, WithdrawalTransaction,
//     TransferTransaction,
//                  StockBuyTransaction, StockSellTransaction>;
//
// class Transaction {
// private:
//   TransactionData data_;
//
// public:
//   template <typename T>
//   Transaction(std::string transaction_id, T &&data)
//       : transaction_id_(std::move(transaction_id)),
//         data_(std::forward<T>(data)),
//         timestamp_(std::chrono::system_clock::now()), completed_(false) {}
//
//   // Visitor pattern for transaction processing
//   template <typename Visitor>
//   auto process(Visitor &&visitor) -> decltype(auto) {
//     return std::visit(std::forward<Visitor>(visitor), data_);
//   }
//
//   bool is_completed() const noexcept { return completed_; }
//   void mark_completed() noexcept { completed_ = true; }
//
//   // Type-safe data access
//   template <typename T> const T *get_as() const noexcept {
//     return std::get_if<T>(&data_);
//   }
// };
