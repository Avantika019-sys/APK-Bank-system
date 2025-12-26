#include "asset/Manager.h"
#include "bank/Account.h"
#include "bank/User.h"
#include <string>
#include <utility>
#include <vector>

#ifndef BANK_BANK_H
#define BANK_BANK_H
namespace bank {
class Bank {
public:
  Bank(std::string name) : name_(name) {}
  // Bank(const Bank &other) = delete;
  // Bank &operator=(const Bank &other) = delete;
  template <typename T>
  void switchToThisBank(Bank *fromBank, std::string accId) {
    if (this == fromBank) {
      throw std::invalid_argument("already on this bank");
    }
    auto it = std::find_if(
        fromBank->users_.begin(), fromBank->users_.end(),
        [&](const asset::Manager<T> &acc) { return acc.getId() == accId; });
    if (it != fromBank->users_.end()) {
      users_.push_back(std::move(*it));
      fromBank->users_.erase(it);
    }
  }
  template <typename... Args> void addUser(Args &&...args) {
    users_.emplace_back(std::forward<Args>(args)...);
  }
  bank::User &getUserByCpr(std::string cpr) {
    auto it = std::find_if(users_.begin(), users_.end(),
                           [&](const User &u) { return u.getCpr() == cpr; });
    if (it == users_.end()) {
      throw std::invalid_argument("Account not found in bank");
    }
    return *it;
  }
  std::string getBankName() const { return name_; }

private:
  std::vector<User> users_;
  std::string name_;
};
} // namespace bank
#endif // BANK_BANK_H
