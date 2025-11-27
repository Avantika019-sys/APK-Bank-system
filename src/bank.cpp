
#include "bank.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

Bank::Bank(std::string name) : name_(name) {}

void Bank::switchToThisBank(Bank &fromBank, std::string accId) {
  auto it =
      std::find_if(fromBank.accounts.begin(), fromBank.accounts.end(),
                   [&](const Account &acc) { return acc.getId() == accId; });
  if (it != fromBank.accounts.end()) {
    accounts.push_back(std::move(*it));
    fromBank.accounts.erase(it);
  }
}

Account &Bank::getAccountById(std::string id) {
  auto it = std::find_if(accounts.begin(), accounts.end(),
                         [&](const Account &acc) { return acc.getId() == id; });
  if (it == accounts.end()) {
    throw std::invalid_argument("Account not found in bank");
  }
  return *it;
}
  std::string Bank::getBankName(){return name_;}

