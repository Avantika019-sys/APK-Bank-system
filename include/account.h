#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include "transaction.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <log.hpp>

class Account {
public:
  Account(std::string name, std::string id);

  Account(Account &&other) noexcept;

  Account &operator=(Account &&other) noexcept;

  ~Account();

  void deposit(int amount);

  void withdraw(int amount);

  void printTransactionHistory() const;

  int getCurrentBalance() const;

  std::string getId() const;
  Account(const Account &other) = delete;
  Account &operator=(const Account &other) = delete;

protected:
  std::vector<moneyTx> moneyTxs_;
  FILE *fptrLogs_;

private:
  std::string name_;
  std::string id_;
  Log log;
};

#endif // BANK_ACCOUNT_H
