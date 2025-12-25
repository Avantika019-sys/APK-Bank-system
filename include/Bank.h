#ifndef BANK_BANK_H
#define BANK_BANK_H
#include "../include/account/AssetAccount.h"
#include <string>
#include <utility>
#include <vector>

template <typename T> class Bank {
public:
  Bank(std::string name) : name_(name) {}
  // Bank(const Bank &other) = delete;
  // Bank &operator=(const Bank &other) = delete;
  void switchToThisBank(Bank *fromBank, std::string accId) {
    if (this == fromBank) {
      throw std::invalid_argument("already on this bank");
    }
    auto it = std::find_if(fromBank->assetAccounts_.begin(),
                           fromBank->assetAccounts_.end(),
                           [&](const bank::account::AssetAccount<T> &acc) {
                             return acc.getId() == accId;
                           });
    if (it != fromBank->assetAccounts_.end()) {
      assetAccounts_.push_back(std::move(*it));
      fromBank->assetAccounts_.erase(it);
    }
  }
  template <typename... Args>
  bank::account::AssetAccount<T> *addAssetAccount(Args &&...args) {
    assetAccounts_.emplace_back(std::forward<Args>(args)...);
    return &assetAccounts_.back();
  }
  bank::account::AssetAccount<T> &getAccountById(std::string id) {
    auto it =
        std::find_if(assetAccounts_.begin(), assetAccounts_.end(),
                     [&](const Account &acc) { return acc.getId() == id; });
    if (it == assetAccounts_.end()) {
      throw std::invalid_argument("Account not found in bank");
    }
    return *it;
  }
  std::string getBankName() const { return name_; }

private:
  std::vector<bank::account::AssetAccount<T>> assetAccounts_;
  std::string name_;
};
#endif // ANK_BANK_H

// check if having 5 accounts max can fulfill a concept that we dont have
