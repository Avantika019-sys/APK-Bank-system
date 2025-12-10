#include "account.h"
#include "transaction.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <sys/types.h>

Account::Account(std::string name, std::string id)
    : logger_(id), name_(name), id_(id) {}

Account::Account(Account &&other) noexcept
    : moneyTxs_(std::move(other.moneyTxs_)), name_(std::move(other.name_)),
      logger_(std::move(other.logger_)) {}

Account &Account::operator=(Account &&other) noexcept {
  if (this != &other) {

    moneyTxs_ = std::move(other.moneyTxs_);
    // name_ = std::move(other.name_);
  }
  return *this;
}

void Account::deposit(uint amount) {
  // add some logs/statistics
  moneyTx tx(amount, moneyTxType::deposit);
  moneyTxs_.push_back(tx);
}

void Account::withdraw(uint amount) {
  int curBalance = getBalance();
  if (curBalance < amount) {
    throw std::invalid_argument("Not enough money on account");
  }
  moneyTx tx(amount, moneyTxType::withdraw);
  moneyTxs_.push_back(tx);
  // add some logs/statistics
}

void Account::printTransactionHistory() const {
  std::for_each(moneyTxs_.begin(), moneyTxs_.end(),
                [](const moneyTx &tx) { std::cout << tx << std::endl; });
}

int Account::getBalance() const {
  int res = std::accumulate(moneyTxs_.begin(), moneyTxs_.end(), 0,
                            [](int acc, const moneyTx &tx) {
                              switch (tx.getType()) {
                              case moneyTxType::deposit:
                                return acc + tx.getAmount();
                              case moneyTxType::withdraw:
                                return acc - tx.getAmount();
                              case moneyTxType::stockPurchase:
                                return acc - tx.getAmount();
                              };
                            });
  return res;
}

std::string Account::getAccountType() const { return type_; }
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
