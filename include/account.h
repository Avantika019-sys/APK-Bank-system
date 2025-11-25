#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include "transaction.h"
#include <stdio.h>
#include <string>
#include <vector>

class Account {
public:
  Account();

  Account(Account &&other) noexcept;

  Account &operator=(Account &&other) noexcept;

  ~Account();

  void deposit(int amount);

  void withdraw(int amount);

  void printTransactionHistory() const;

  int getCurrentBalance() const;

  std::string getId() const;

protected:
  std::vector<moneyTx> moneyTxs_;
  FILE *fptrLogs_;

private:
  Account(const Account &other);

  Account &operator=(const Account &other);

  std::string id_;
};

#endif // BANK_ACCOUNT_H
