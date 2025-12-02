#pragma once
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <memory_resource>
#include "concepts.h"
#include "exceptions.h"

namespace banking {
    
// Base Account interface
class Account {
public:
    virtual ~Account() = default;
    
    virtual bool deposit(double amount) noexcept = 0;
    virtual bool withdraw(double amount) = 0;
    virtual double get_balance() const noexcept = 0;
    virtual std::string get_account_number() const = 0;
    virtual std::string get_account_type() const = 0;
    
    // Template method pattern
    bool transfer(double amount, Account& to_account) {
        if (withdraw(amount)) {
            if (to_account.deposit(amount)) {
                return true;
            } else {
                // Rollback - strong exception guarantee
                deposit(amount);
                throw exceptions::InvalidTransactionException("Transfer failed");
            }
        }
        return false;
    }
};


template<typename Derived>
class AccountWithStatistics {
private:
    size_t transaction_count_ = 0;
    double total_deposited_ = 0.0;
    double total_withdrawn_ = 0.0;
    
protected:
    void record_deposit(double amount) noexcept {
        ++transaction_count_;
        total_deposited_ += amount;
    }
    
    void record_withdrawal(double amount) noexcept {
        ++transaction_count_;
        total_withdrawn_ += amount;
    }
    
public:
    size_t get_transaction_count() const noexcept { return transaction_count_; }
    double get_total_deposited() const noexcept { return total_deposited_; }
    double get_total_withdrawn() const noexcept { return total_withdrawn_; }
    
    double get_net_flow() const noexcept {
        return total_deposited_ - total_withdrawn_;
    }
};

// Savings Account with interest
class SavingsAccount final : public Account, public AccountWithStatistics<SavingsAccount> {
private:
    std::string account_number_;
    double balance_;
    double interest_rate_;
    
public:
    using AccountType = struct SavingsTag {};
    
    SavingsAccount(std::string account_number, double initial_balance = 0.0, 
                   double interest_rate = 0.02)
        : account_number_(std::move(account_number))
        , balance_(initial_balance)
        , interest_rate_(interest_rate) {
        if (initial_balance < 0) {
            throw exceptions::InvalidTransactionException("Initial balance cannot be negative");
        }
    }
    
    bool deposit(double amount) noexcept override {
        if (amount > 0) {
            balance_ += amount;
            record_deposit(amount);
            return true;
        }
        return false;
    }
    
    bool withdraw(double amount) override {
        if (amount > 0 && amount <= balance_) {
            balance_ -= amount;
            record_withdrawal(amount);
            return true;
        } else if (amount > balance_) {
            throw exceptions::InsufficientFundsException(balance_, amount);
        }
        return false;
    }
    
    double get_balance() const noexcept override { return balance_; }
    std::string get_account_number() const override { return account_number_; }
    std::string get_account_type() const override { return "Savings"; }
    
    void apply_interest() noexcept {
        double interest = balance_ * interest_rate_;
        balance_ += interest;
        record_deposit(interest);
    }
    
    double get_interest_rate() const noexcept { return interest_rate_; }
};

// Checking Account with overdraft protection
class CheckingAccount final : public Account, public AccountWithStatistics<CheckingAccount> {
private:
    std::string account_number_;
    double balance_;
    double overdraft_limit_;
    
public:
    using AccountType = struct CheckingTag {};
    
    CheckingAccount(std::string account_number, double initial_balance = 0.0,
                    double overdraft_limit = 500.0)
        : account_number_(std::move(account_number))
        , balance_(initial_balance)
        , overdraft_limit_(overdraft_limit) {}
    
    bool deposit(double amount) noexcept override {
        if (amount > 0) {
            balance_ += amount;
            record_deposit(amount);
            return true;
        }
        return false;
    }
    
    bool withdraw(double amount) override {
        if (amount > 0 && (balance_ - amount) >= -overdraft_limit_) {
            balance_ -= amount;
            record_withdrawal(amount);
            return true;
        } else if (amount > 0 && (balance_ - amount) < -overdraft_limit_) {
            throw exceptions::InsufficientFundsException(balance_, amount);
        }
        return false;
    }
    
    double get_balance() const noexcept override { return balance_; }
    std::string get_account_number() const override { return account_number_; }
    std::string get_account_type() const override { return "Checking"; }
    
    double get_overdraft_limit() const noexcept { return overdraft_limit_; }
};

// Account factory with variadic templates
class AccountFactory {
public:
    template<typename AccountType, typename... Args>
    static std::unique_ptr<AccountType> create_account(Args&&... args) {
        return std::make_unique<AccountType>(std::forward<Args>(args)...);
    }
    
    // Type-safe account creation using concepts
    template<concepts::AccountLike AccountType, typename... Args>
    static auto create(Args&&... args) {
        return std::make_unique<AccountType>(std::forward<Args>(args)...);
    }
};

} // namespace banking