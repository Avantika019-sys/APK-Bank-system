#pragma once
#include <stdexcept>
#include <string>
#include <string_view>

namespace banking::exceptions {
    
// Base exception for banking system
class BankingException : public std::runtime_error {
public:
    explicit BankingException(const std::string& msg) 
        : std::runtime_error(msg) {}
        
    virtual ~BankingException() = default;
};

// Specific exception types using hierarchy
class InsufficientFundsException : public BankingException {
private:
    double current_balance_;
    double requested_amount_;
    
public:
    InsufficientFundsException(double balance, double amount)
        : BankingException("Insufficient funds for transaction"),
          current_balance_(balance), requested_amount_(amount) {}
          
    double get_balance() const noexcept { return current_balance_; }
    double get_requested_amount() const noexcept { return requested_amount_; }
    
    const char* what() const noexcept override {
        static std::string msg = "Insufficient funds: Balance=" + 
            std::to_string(current_balance_) + 
            ", Requested=" + std::to_string(requested_amount_);
        return msg.c_str();
    }
};

class InvalidTransactionException : public BankingException {
public:
    explicit InvalidTransactionException(const std::string& msg)
        : BankingException(msg) {}
};

class AccountNotFoundException : public BankingException {
private:
    std::string account_id_;
    
public:
    AccountNotFoundException(const std::string& account_id)
        : BankingException("Account not found"), account_id_(account_id) {}
        
    const std::string& get_account_id() const noexcept { return account_id_; }
};

class StockTradingException : public BankingException {
public:
    explicit StockTradingException(const std::string& msg)
        : BankingException(msg) {}
};

} // namespace banking::exceptions