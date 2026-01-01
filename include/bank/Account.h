#include "bank/tx/Deposit.h"
#include "bank/tx/Withdraw.h"
#include "bank/tx/asset/Purchase.h"
#include "bank/tx/asset/Sale.h"
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
  Account() = default;
  void deposit(double amount);

  void withdraw(double amount);

  void printTransactionHistory() const;

  double getBalance() const;
  void addTransaction(txVariant &&tx);

  void generateBankStatement() const;
  Account(Account &&other) noexcept;
  Account &operator=(Account &&other) noexcept;
  Account(const Account &other) = delete;
  Account &operator=(const Account &other) = delete;

private:
  std::vector<txVariant> txs_;
  mutable std::mutex mtx_;
  double balance_ = 0;
};
} // namespace bank
#endif // BANK_ACCOUNT_H
