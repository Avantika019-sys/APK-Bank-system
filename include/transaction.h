
#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>
#include <iostream>
#include <memory_resource>
#include <string>
#include <variant>
#include <vector>
using namespace std;

struct stockPurchaseDetails {
  std::string stockName_;
  uint stocksBought_;
  uint pricePerStock_;
};
struct stockSellDetails {
  std::string stockName_;
  uint stocksSold_;
  uint pricePerStock_;
};
struct withdrawDetails {
  uint amountWithdrawn_;
};
struct depositDetails {
  uint amountDepositted_;
};
// enum stock, buy sell
typedef std::variant<stockPurchaseDetails, stockSellDetails, withdrawDetails,
                     depositDetails>
    details;
;

class Tx {

public:
  Tx(details d);
  const details &getDetails() const;
  std::chrono::system_clock::time_point getCreatedAt() const;

private:
  std::pmr::memory_resource *memRes = std::pmr::get_default_resource();
  std::chrono::system_clock::time_point createdAt_;
  std::string from_account;
  std::string to_account;
  details details_;
  // std::string transaction_id_;
};
struct ToString {
  std::string operator()(const stockSellDetails &arg) {
    return "transaction type: stock sale\nPrice per stock: " +
           std::to_string(arg.pricePerStock_) +
           "\nAmount of stocks sold: " + std::to_string(arg.pricePerStock_) +
           "\nStock name: " + arg.stockName_;
  }
  std::string operator()(stockPurchaseDetails arg) {
    return "transaction type: stock purchase\nPrice per stock: " +
           std::to_string(arg.pricePerStock_) +
           "\nAmount of stocks bought: " + std::to_string(arg.stocksBought_) +
           "\nStock name: " + arg.stockName_;
  }
  std::string operator()(withdrawDetails arg) {
    return "transaction type: withdrawal\nAmount withdrawn: " +
           std::to_string(arg.amountWithdrawn_);
  }
  std::string operator()(depositDetails arg) {
    return "transaction type: deposit\nAmount depositted: " +
           std::to_string(arg.amountDepositted_);
  }
};
//
// private:
//   std::chrono::system_clock::time_point createdAt_;
//   int amount_;
//   std::string stockName_;
// };

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
