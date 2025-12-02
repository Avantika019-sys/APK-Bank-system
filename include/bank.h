#ifndef BANK_BANK_H
#define BANK_BANK_H
#include "account.h"
#include <string>
#include <utility>
#include <vector>

class Bank {
public:
  Bank(std::string name);
  Bank(const Bank &other) = delete;
  Bank &operator=(const Bank &other) = delete;
  void switchToThisBank(Bank &fromBank, std::string accId);
  template <typename... Args> void addAccount(Args &&...args) {
    accounts.emplace_back(std::forward<Args>(args)...);
  }
  Account &getAccountById(std::string id);
  std::string getBankName();

private:
  std::vector<Account> accounts;
  std::string name_;
};
#endif // ANK_BANK_H
