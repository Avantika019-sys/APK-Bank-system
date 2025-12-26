#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include "Logger.h"
#include "Tx.h"
#include <string>
#include <sys/types.h>
#include <vector>

class Account {
public:
  Account(std::string name, std::string id);

  void deposit(int amount);

  void withdraw(int amount);

  void printTransactionHistory() const;

  int getBalance() const;

  std::string getId() const;

  Account(Account &&other) noexcept;
  Account &operator=(Account &&other) noexcept;

protected:
  std::pmr::vector<Tx> txs_{&pool_};
  std::pmr::monotonic_buffer_resource pool_;
  Logger logger_;

private:
  std::string name_;
  std::string id_;
  // max 5 accounts
};
//
// class AccountFactory {
// public:
//   template <typename AccountType, typename... Args>
//   static std::unique_ptr<AccountType> create_account(Args &&...args) {
//     return std::make_unique<AccountType>(std::forward<Args>(args)...);
//   }
//
//   // Type-safe account creation using concepts
//   template <concepts::AccountLike AccountType, typename... Args>
//   static auto create(Args &&...args) {
//     return std::make_unique<AccountType>(std::forward<Args>(args)...);
//   }
// };
//
#endif // BANK_ACCOUNT_H
