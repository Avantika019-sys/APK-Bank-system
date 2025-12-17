#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include "logger.h"
#include "transaction.h"
#include <string>
#include <sys/types.h>
#include <vector>

class Account {
public:
  Account(std::string name, std::string id);

  // Account(Account &&other) noexcept;
  //
  // Account &operator=(Account &&other) noexcept;

  void deposit(uint amount);

  void withdraw(uint amount);

  void printTransactionHistory() const;

  int getBalance() const;

  std::string getAccountType() const;

  std::string getId() const;
  // Account(const Account &other) = delete;
  // Account &operator=(const Account &other) = delete;

protected:
  std::vector<Tx> txs_;
  logger logger_;

private:
  std::string name_;
  std::string id_;
  std::string type_;
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
