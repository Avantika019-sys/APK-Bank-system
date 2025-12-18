#pragma once
#include "account.h"
#include "concepts.h"
#include <memory>
#include <memory_resource>
#include <string>
#include <unordered_map>
#include <vector>

namespace banking {

class CreditCard;
class StockPortfolio;

class User {
private:
  std::string user_id_;
  std::string name_;
  std::string email_;

  // Using PMR for efficient memory management
  std::pmr::vector<std::unique_ptr<Account>> accounts_;
  std::pmr::vector<std::unique_ptr<CreditCard>> cards_;
  std::pmr::unordered_map<std::string, std::unique_ptr<StockPortfolio>>
      portfolios_;

public:
  User(std::string user_id, std::string name, std::string email,
       std::pmr::memory_resource *pool = std::pmr::get_default_resource())
      : user_id_(std::move(user_id)), name_(std::move(name)),
        email_(std::move(email)), accounts_(pool), cards_(pool),
        portfolios_(pool) {}

  // Rule of Five with move semantics
  User(const User &) = delete;
  User &operator=(const User &) = delete;

  User(User &&other) noexcept = default;
  User &operator=(User &&other) noexcept = default;

  ~User() = default;

  // Getters
  const std::string &get_user_id() const noexcept { return user_id_; }
  const std::string &get_name() const noexcept { return name_; }
  const std::string &get_email() const noexcept { return email_; }

  // Account management with perfect forwarding
  template <typename AccountType, typename... Args>
    requires concepts::AccountLike<AccountType>
  AccountType &create_account(Args &&...args) {
    auto account = std::make_unique<AccountType>(std::forward<Args>(args)...);
    auto &ref = *account;
    accounts_.push_back(std::move(account));
    return ref;
  }

  // Variadic template for bulk operations
  template <typename... AccountTypes>
  void add_accounts(AccountTypes &&...accounts) {
    (accounts_.push_back(std::make_unique<std::decay_t<AccountTypes>>(
         std::forward<AccountTypes>(accounts))),
     ...);
  }

  // Range-based for loop support
  auto begin() const { return accounts_.begin(); }
  auto end() const { return accounts_.end(); }

  // Constexpr if for type-specific operations
  template <typename T> void process_accounts() {
    for (const auto &account : accounts_) {
      if (auto *specific_account = dynamic_cast<T *>(account.get())) {
        if constexpr (requires { specific_account->apply_interest(); }) {
          specific_account->apply_interest();
        }
      }
    }
  }

  // SFINAE for account type checking
  template <typename T> auto get_accounts_of_type() const {
    std::vector<T *> result;
    for (const auto &account : accounts_) {
      if (auto *typed_account = dynamic_cast<T *>(account.get())) {
        result.push_back(typed_account);
      }
    }
    return result;
  }
};

} // namespace banking
