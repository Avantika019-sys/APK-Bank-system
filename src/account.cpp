#include "account.h"
#include "Tx.h"
#include "TxDetails.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <sys/types.h>

Account::Account(std::string name, std::string id)
    : logger_(id), name_(name), id_(id) {}

// Account::Account(Account &&other) noexcept
//     : moneyTxs_(std::move(other.moneyTxs_)), name_(std::move(other.name_)),
//       logger_(std::move(other.logger_)) {}
//
// Account &Account::operator=(Account &&other) noexcept {
//   if (this != &other) {
//
//     moneyTxs_ = std::move(other.moneyTxs_);
//     // name_ = std::move(other.name_);
//   }
//   return *this;
// }

void Account::deposit(int amount) {
  // add some logs/statistics
  Tx tx(depositDetails{amount});
  txs_.push_back(tx);
  logger_.log("successfully made deposit", level::INFO, "transaction", tx);
}

void Account::withdraw(int amount) {
  int curBalance = getBalance();
  if (curBalance < amount) {
    logger_.log("failed to withdraw because insufficient funds", level::ERROR,
                "withdraw_amount", amount);
    throw std::invalid_argument("Not enough money on account");
  }
  txs_.emplace_back(withdrawDetails{amount});
  // add some logs/statistics
}

void Account::printTransactionHistory() const {
  std::for_each(txs_.begin(), txs_.end(),
                [](const Tx &tx) { std::cout << tx << std::endl; });
}

int Account::getBalance() const {
  struct getAmount {
    int operator()(const stockSellDetails &arg) {
      return arg.pricePerStock_ * arg.stocksSold_;
    }
    int operator()(const stockPurchaseDetails &arg) {
      return -(arg.pricePerStock_ * arg.stocksBought_);
    }
    int operator()(const withdrawDetails &arg) { return -arg.amountWithdrawn_; }
    int operator()(const depositDetails &arg) { return arg.amountDepositted_; }
  };
  int res =
      std::accumulate(txs_.begin(), txs_.end(), 0, [](int acc, const Tx &tx) {
        return std::visit(getAmount{}, tx.getDetails());
      });
  return res;
}

std::string Account::getId() const { return id_; }
//
// bool transfer(double amount, Account &to_account) {
//   if (withdraw(amount)) {
//     if (to_account.deposit(amount)) {
//       return true;
//     } else {
//       // Rollback - strong exception guarantee
//       deposit(amount);
//       throw exceptions::InvalidTransactionException("Transfer failed");
//     }
//   }
//   return false;
// }
