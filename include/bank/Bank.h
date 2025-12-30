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

private:
  std::vector<User> users_;
};
} // namespace bank
#endif // BANK_BANK_H
