#include "bank/tx/Deposit.h"
#include "bank/tx/Withdraw.h"
#include "bank/tx/asset/Purchase.h"
#include "bank/tx/asset/Sale.h"
#include "util/Logger.h"
#include <concepts>
#include <memory_resource>
#include <string>
#include <sys/types.h>
#include <variant>
#include <vector>
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
namespace bank {
typedef std::variant<tx::Deposit, tx::Withdraw, tx::asset::Purchase,
                     tx::asset::Sale>
    txVariant;
class Account {
public:
  Account(util::Logger *logger);

  void deposit(double amount);

  void withdraw(double amount);

  void printTransactionHistory() const;

  int getBalance() const;
  template <typename... Args> void addTransaction(Args &&...args) {
    std::lock_guard<std::mutex> lock(mtx_);
    txs_.emplace_back(std::forward<Args>(args)..., &pool_);
  }

  Account(Account &&other) noexcept;
  Account &operator=(Account &&other) noexcept;

private:
  std::pmr::vector<txVariant> txs_{&pool_};
  std::pmr::unsynchronized_pool_resource pool_;
  util::Logger *logger_;
  std::mutex mtx_;
};
} // namespace bank
#endif // BANK_ACCOUNT_H
