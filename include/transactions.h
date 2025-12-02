#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <variant>
#include <memory_resource>
#include <algorithm>
#include "concepts.h"
#include "exceptions.h"

namespace banking {
    
// Transaction types
struct DepositTransaction {
    double amount;
    std::string description;
};

struct WithdrawalTransaction {
    double amount;
    std::string description;
};

struct TransferTransaction {
    double amount;
    std::string from_account;
    std::string to_account;
    std::string description;
};

struct StockBuyTransaction {
    std::string symbol;
    int quantity;
    double price_per_share;
};

struct StockSellTransaction {
    std::string symbol;
    int quantity;
    double price_per_share;
};

using TransactionData = std::variant<
    DepositTransaction,
    WithdrawalTransaction,
    TransferTransaction,
    StockBuyTransaction,
    StockSellTransaction
>;

class Transaction {
private:
    std::string transaction_id_;
    TransactionData data_;
    std::chrono::system_clock::time_point timestamp_;
    bool completed_;
    
public:
    template<typename T>
    Transaction(std::string transaction_id, T&& data)
        : transaction_id_(std::move(transaction_id))
        , data_(std::forward<T>(data))
        , timestamp_(std::chrono::system_clock::now())
        , completed_(false) {}
    
    // Visitor pattern for transaction processing
    template<typename Visitor>
    auto process(Visitor&& visitor) -> decltype(auto) {
        return std::visit(std::forward<Visitor>(visitor), data_);
    }
    
    const std::string& get_id() const noexcept { return transaction_id_; }
    const auto& get_timestamp() const noexcept { return timestamp_; }
    bool is_completed() const noexcept { return completed_; }
    void mark_completed() noexcept { completed_ = true; }
    
    // Type-safe data access
    template<typename T>
    const T* get_as() const noexcept {
        return std::get_if<T>(&data_);
    }
};

// Transaction processor with strategy pattern
class TransactionProcessor {
private:
    std::pmr::vector<Transaction> pending_transactions_;
    std::pmr::vector<Transaction> completed_transactions_;
    
public:
    explicit TransactionProcessor(std::pmr::memory_resource* pool = std::pmr::get_default_resource())
        : pending_transactions_(pool)
        , completed_transactions_(pool) {}
    
    template<typename T>
    void add_transaction(T&& transaction) {
        pending_transactions_.emplace_back(std::forward<T>(transaction));
    }
    
    // Process transactions with exception safety
    void process_pending() {
        for (auto& transaction : pending_transactions_) {
            try {
                bool success = transaction.process([this](const auto& data) {
                    return process_impl(data);
                });
                
                if (success) {
                    transaction.mark_completed();
                    completed_transactions_.push_back(std::move(transaction));
                }
            } catch (const exceptions::BankingException& e) {
                std::cerr << "Transaction failed: " << e.what() << "\n";
            }
        }
        
        // Remove processed transactions
        pending_transactions_.erase(
            std::remove_if(pending_transactions_.begin(), pending_transactions_.end(),
                [](const Transaction& t) { return t.is_completed(); }),
            pending_transactions_.end());
    }
    
private:
    bool process_impl(const DepositTransaction& deposit) {
        std::cout << "Processing deposit: $" << deposit.amount << " - " 
                  << deposit.description << "\n";
        return true;
    }
    
    bool process_impl(const WithdrawalTransaction& withdrawal) {
        std::cout << "Processing withdrawal: $" << withdrawal.amount << " - " 
                  << withdrawal.description << "\n";
        return true;
    }
    
    bool process_impl(const TransferTransaction& transfer) {
        std::cout << "Processing transfer: $" << transfer.amount << " from " 
                  << transfer.from_account << " to " << transfer.to_account << "\n";
        return true;
    }
    
    bool process_impl(const StockBuyTransaction& buy) {
        std::cout << "Processing stock buy: " << buy.quantity << " shares of " 
                  << buy.symbol << " at $" << buy.price_per_share << "\n";
        return true;
    }
    
    bool process_impl(const StockSellTransaction& sell) {
        std::cout << "Processing stock sell: " << sell.quantity << " shares of " 
                  << sell.symbol << " at $" << sell.price_per_share << "\n";
        return true;
    }
};

} // namespace banking