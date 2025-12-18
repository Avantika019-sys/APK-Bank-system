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

  void deposit(uint amount);

  void withdraw(uint amount);

  void printTransactionHistory() const;

  int getBalance() const;

  std::string getAccountType() const;

  std::string getId() const;

protected:
  std::pmr::vector<Tx> txs_{&pool_};
  std::pmr::monotonic_buffer_resource pool_;
  logger logger_;

private:
  std::string name_;
  std::string id_;
  std::string type_;
  // max 5 accounts
};
struct getTransactionAmount {
  int operator()(const stockSellDetails &arg) {
    return arg.pricePerStock_ * arg.stocksSold_;
  }
  int operator()(const stockPurchaseDetails &arg) {
    return -(arg.pricePerStock_ * arg.stocksBought_);
  }
  int operator()(const withdrawDetails &arg) { return -arg.amountWithdrawn_; }
  int operator()(const depositDetails &arg) { return arg.amountDepositted_; }
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
