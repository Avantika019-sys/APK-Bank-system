#include "bank/Account.h"
#include "bank/Tx.h"
#include "bank/TxDetails.h"
#include "util/Logger.h"
#include <algorithm>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <sys/types.h>
namespace bank {
Account::Account(util::Logger *logger) : logger_(logger) {}

Account::Account(Account &&other) noexcept
    : txs_(std::move(other.txs_)), logger_(std::move(other.logger_)) {}

Account &Account::operator=(Account &&other) noexcept {
  if (this != &other) {
    txs_ = std::move(other.txs_);
  }
  return *this;
}

void Account::deposit(int amount) {
  // add some logs/statistics
  Tx tx(depositDetails{amount}, &pool_);
  txs_.push_back(tx);
  logger_.log("successfully made deposit", util::level::INFO,
              util::field("transaction", tx));
}

void Account::withdraw(int amount) {
  int curBalance = getBalance();
  if (curBalance < amount) {
    logger_.log("failed to withdraw because insufficient funds",
                util::level::ERROR, util::field("withdraw amount", amount),
                util::field("current balance", curBalance));
    throw std::invalid_argument("Not enough money on account");
  }
  txs_.emplace_back(withdrawDetails{amount}, &pool_);
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

void Account::addTransaction(details &&txDetail) {
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.emplace_back(std::move(txDetail), &pool_);
}
} // namespace bank
