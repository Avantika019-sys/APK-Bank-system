#include "tx.hpp"
#include <sys/types.h>
#include <variant>
#include <vector>
#ifndef EXCHANGE_ACCOUNT_H
#define EXCHANGE_ACCOUNT_H
namespace exchange {
typedef std::variant<tx::Deposit, tx::Withdraw, tx::Purchase, tx::Sale>
    txVariant;
class Account {
public:
  Account() = default;
  void deposit(double amount);

  void withdraw(double amount);

  void printTransactionHistory() const;

  double getBalance() const;
  void printBalance() const;
  void addTransaction(txVariant &&tx);

  void generateAccountStatement() const;
  Account(Account &&other) = delete;
  Account &operator=(Account &&other) = delete;
  Account(const Account &other) = delete;
  Account &operator=(const Account &other) = delete;

private:
  std::vector<txVariant> txs_;
  mutable std::mutex mtx_;
  double balance_ = 0;
};
} // namespace exchange
#endif // EXCHANGE_ACCOUNT_H
