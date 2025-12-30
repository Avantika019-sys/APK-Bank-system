#include "bank/Account.h"
#include "util/Logger.h"
#include <algorithm>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <utility>
#include <variant>
namespace bank {
Account::Account(util::Logger *logger) : logger_(logger), pool_() {}

Account::Account(Account &&other) noexcept
    : txs_(std::move(other.txs_)), logger_(std::move(other.logger_)) {}

Account &Account::operator=(Account &&other) noexcept {
  if (this != &other) {
    txs_ = std::move(other.txs_);
  }
  return *this;
}

void Account::deposit(double amount) {
  txs_.emplace_back(tx::Deposit{amount, &pool_});
  logger_->log("successfully made deposit", util::level::INFO,
               util::field("amount", amount));
}

void Account::withdraw(double amount) {
  int curBalance = getBalance();
  if (curBalance < amount) {
    logger_->log("failed to withdraw because insufficient funds",
                 util::level::ERROR, util::field("withdraw amount", amount),
                 util::field("current balance", curBalance));
    throw std::invalid_argument("Not enough money on account");
  }
  txs_.emplace_back(tx::Deposit{amount, &pool_});
}

void Account::printTransactionHistory() const {
  std::for_each(txs_.begin(), txs_.end(), [](const txVariant &txV) {
    std::visit(
        [](const auto &tx) {
          // static_assert(util::toStringable<std::decay_t<decltype(tx)>>::value);
          std::cout << tx.toString() << std::endl;
        },
        txV);
  });
}

// template <typename T>
// concept totalField = requires(T t) {
//   { t.total } -> std::same_as<double>;
// };
int Account::getBalance() const {
  int res = std::accumulate(txs_.begin(), txs_.end(), 0,
                            [](double acc, const txVariant &txV) {
                              return acc + std::visit(
                                               [](const auto &tx) {
                                                 // static_assert(totalField<std::decay_t<decltype(tx)>>);
                                                 return tx.total;
                                               },
                                               txV);
                            });
  return res;
}
} // namespace bank
