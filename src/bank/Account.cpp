#include "bank/Account.h"
#include "util/Logger.h"
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <utility>
#include <variant>
namespace bank {
Account::Account(util::Logger *logger) : logger_(logger), balance_(0) {}

Account::Account(Account &&other) noexcept
    : txs_(std::move(other.txs_)), logger_(std::move(other.logger_)) {}

Account &Account::operator=(Account &&other) noexcept {
  if (this != &other) {
    txs_ = std::move(other.txs_);
  }
  return *this;
}

void Account::deposit(double amount) {
  balance_ = amount;
  txs_.emplace_back(tx::Deposit{amount});
  logger_->log("successfully made deposit", util::level::INFO,
               util::field("amount", amount));
}

void Account::withdraw(double amount) {
  if (balance_ < amount) {
    logger_->log("failed to withdraw because insufficient funds",
                 util::level::ERROR, util::field("withdraw amount", amount),
                 util::field("current balance", balance_));
    throw std::invalid_argument("Not enough money on account");
  }
  balance_ = -amount;
  txs_.emplace_back(tx::Deposit{amount});
}

void Account::printTransactionHistory() const {
  std::cout << "---------------------------" << std::endl;
  std::cout << "TRANSACTION HISTORY:\n" << std::endl;
  std::for_each(txs_.begin(), txs_.end(), [](const txVariant &txV) {
    std::visit([](const auto &tx) { std::cout << tx.toString() << std::endl; },
               txV);
    std::cout << "\n";
  });
  std::cout << "---------------------------" << std::endl;
}

void Account::generateBankStatement() {
  auto now = std::chrono::system_clock::now();
  std::string nowStr =
      std::format("../statements/{:%Y-%m-%d-%H:%M:%S}-BANK-STATEMENT", now);
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
void Account::addTransaction(txVariant &&tx) {
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.push_back(std::move(tx));
}
} // namespace bank
