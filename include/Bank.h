#ifndef BANK_BANK_H
#define BANK_BANK_H
#include "stock/StockAccount.h"
#include <string>
#include <utility>
#include <vector>

class Bank {
public:
  Bank(std::string name);
  // Bank(const Bank &other) = delete;
  // Bank &operator=(const Bank &other) = delete;
  void switchToThisBank(Bank *fromBank, std::string accId);
  template <typename... Args>
  bank::stock::StockAccount *addStockAccount(Args &&...args) {
    stockAccounts.emplace_back(std::forward<Args>(args)...);
    return &stockAccounts.back();
  }
  bank::stock::StockAccount &getAccountById(std::string id);
  std::string getBankName() const;

private:
  std::vector<bank::stock::StockAccount> stockAccounts;
  std::string name_;
};
#endif // ANK_BANK_H

// check if having 5 accounts max can fulfill a concept that we dont have
