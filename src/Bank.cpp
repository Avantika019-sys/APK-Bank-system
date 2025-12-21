
#include "Bank.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

Bank::Bank(std::string name) : name_(name) {}

void Bank::switchToThisBank(Bank *fromBank, std::string accId) {
  if (this == fromBank) {
    throw std::invalid_argument("already on this bank");
  }
  auto it = std::find_if(fromBank->stockAccounts.begin(),
                         fromBank->stockAccounts.end(),
                         [&](const bank::stock::StockAccount &acc) {
                           return acc.getId() == accId;
                         });
  if (it != fromBank->stockAccounts.end()) {
    stockAccounts.push_back(std::move(*it));
    fromBank->stockAccounts.erase(it);
  }
}

bank::stock::StockAccount &Bank::getAccountById(std::string id) {
  auto it = std::find_if(stockAccounts.begin(), stockAccounts.end(),
                         [&](const Account &acc) { return acc.getId() == id; });
  if (it == stockAccounts.end()) {
    throw std::invalid_argument("Account not found in bank");
  }
  return *it;
}
std::string Bank::getBankName() const { return name_; }
