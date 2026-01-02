#include "exchange/Account.h"
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <utility>
#include <variant>
namespace exchange {

void Account::deposit(double amount) {
  std::lock_guard<std::mutex> lock(mtx_);
  balance_ = amount;
  txs_.emplace_back(tx::Deposit{amount});
}

void Account::withdraw(double amount) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (balance_ < amount) {
    throw std::invalid_argument("Not enough money on account");
  }
  balance_ = -amount;
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

void Account::generateAccountStatement() const {
  auto now = std::chrono::system_clock::now();
  std::string nowStr =
      std::format("../statements/{:%Y-%m-%d-%H:%M:%S}-ACCOUNT-STATEMENT", now);
  std::ofstream file;
  file.open(nowStr);
  std::array<std::byte, 1024> buf;
  std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size()};

  std::lock_guard<std::mutex> lock(mtx_);
  std::for_each(txs_.begin(), txs_.end(), [&](const txVariant &txV) {
    std::visit(
        [&](const auto &tx) {
          std::pmr::string txStr{tx.toString() + "\n", &mbr};
          file << txStr;
          mbr.release();
        },
        txV);
  });
}
double Account::getBalance() const { return balance_; }
void Account::printBalance() const {
  std::cout << "---------------------------" << std::endl;
  std::cout << "Current balance: " << balance_ << std::endl;
}
void Account::addTransaction(txVariant &&tx) {
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.push_back(std::move(tx));
}
} // namespace exchange
