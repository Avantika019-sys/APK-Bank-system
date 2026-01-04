#include "exchange/Account.h"
#include "exchange/currency/DKK.h"
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <utility>
#include <variant>
#include <vector>
namespace exchange {

Account::Account() : balance_(0) {}
void Account::deposit(currency::DKK amount) {
  std::lock_guard<std::mutex> lock(mtx_);
  balance_ = amount;
  txs_.emplace_back(tx::Deposit{amount});
}

void Account::withdraw(currency::DKK amount) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (balance_.value() < amount.value()) {
    throw std::invalid_argument("Not enough money on account");
  }
  balance_ -= amount;
  txs_.emplace_back(tx::Deposit{amount});
}

void Account::printTransactionHistory() const {
  std::cout << "---------------------------" << std::endl;
  std::cout << "TRANSACTION HISTORY:\n" << std::endl;
  std::lock_guard<std::mutex> lock(mtx_);
  std::for_each(txs_.begin(), txs_.end(), [](const txVariant &txV) {
    std::visit([](const auto &tx) { std::cout << tx.toString() << std::endl; },
               txV);
    std::cout << "\n";
  });
}

currency::DKK Account::getBalance() const {
  std::lock_guard<std::mutex> lock(mtx_);
  return balance_;
}
void Account::printBalance() const {
  std::cout << "---------------------------" << std::endl;
  std::cout << "Current balance: " << balance_ << std::endl;
}
void Account::addTransaction(txVariant &&txV) {
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.push_back(std::move(txV));
  auto total = std::visit([](const auto &tx) { return tx.total; }, txV);
  balance_ += total;
}
} // namespace exchange
