#ifndef BANK_BANK_H
#define BANK_BANK_H
#include "account.h"
#include <string>
#include <utility>
#include <vector>

class Bank {
public:
  void switchToThisBank(Bank &fromBank, std::string accId);
  template <typename... Args> void addAccount(Args &&...args) {
    accounts.emplace(std::forward<Args>(args)...);
  }
  Account &getAccountById(std::string id);

private:
  std::vector<Account> accounts;
};
#endif // ANK_BANK_H
