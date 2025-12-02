#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <chrono>
#include <memory_resource>
#include <concepts>
#include "concepts.h"
#include "exceptions.h"

namespace banking {
    
struct Stock {
    std::string symbol;
    std::string name;
    double current_price;
    double day_high;
    double day_low;
    
    Stock(std::string sym, std::string n, double price)
        : symbol(std::move(sym))
        , name(std::move(n))
        , current_price(price)
        , day_high(price)
        , day_low(price) {}
};

class StockOrder {
public:
    enum class Type { BUY, SELL };
    enum class Status { PENDING, EXECUTED, CANCELLED, FAILED };
    
private:
    std::string order_id_;
    std::string symbol_;
    Type type_;
    int quantity_;
    double limit_price_;
    Status status_;
    std::chrono::system_clock::time_point timestamp_;
    
public:
    StockOrder(std::string order_id, std::string symbol, Type type, 
               int quantity, double limit_price)
        : order_id_(std::move(order_id))
        , symbol_(std::move(symbol))
        , type_(type)
        , quantity_(quantity)
        , limit_price_(limit_price)
        , status_(Status::PENDING)
        , timestamp_(std::chrono::system_clock::now()) {}
    
    // Move semantics
    StockOrder(StockOrder&&) = default;
    StockOrder& operator=(StockOrder&&) = default;
    
    // Getters
    const std::string& get_order_id() const noexcept { return order_id_; }
    const std::string& get_symbol() const noexcept { return symbol_; }
    Type get_type() const noexcept { return type_; }
    int get_quantity() const noexcept { return quantity_; }
    double get_limit_price() const noexcept { return limit_price_; }
    Status get_status() const noexcept { return status_; }
    
    void execute() noexcept { status_ = Status::EXECUTED; }
    void cancel() noexcept { status_ = Status::CANCELLED; }
    void fail() noexcept { status_ = Status::FAILED; }
};

class StockPortfolio {
private:
    std::string portfolio_id_;
    std::pmr::unordered_map<std::string, int> holdings_;
    std::pmr::vector<StockOrder> order_history_;
    double cash_balance_;
    
public:
    explicit StockPortfolio(std::string portfolio_id, 
                           std::pmr::memory_resource* pool = std::pmr::get_default_resource())
        : portfolio_id_(std::move(portfolio_id))
        , holdings_(pool)
        , order_history_(pool)
        , cash_balance_(0.0) {}
    
    // Perfect forwarding for stock operations
    template<typename... Args>
    void place_buy_order(Args&&... args) {
        order_history_.emplace_back(std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void place_sell_order(Args&&... args) {
        order_history_.emplace_back(std::forward<Args>(args)...);
    }
    
    // Buy stock with strong exception guarantee
    bool buy_stock(const Stock& stock, int quantity, double max_price) {
        double total_cost = stock.current_price * quantity;
        
        if (total_cost > cash_balance_) {
            throw exceptions::InsufficientFundsException(cash_balance_, total_cost);
        }
        
        if (stock.current_price > max_price) {
            throw exceptions::StockTradingException("Current price exceeds maximum price");
        }
        
        // Simulate transaction
        cash_balance_ -= total_cost;
        holdings_[stock.symbol] += quantity;
        
        return true;
    }
    
    // Sell stock
    bool sell_stock(const std::string& symbol, int quantity, double min_price) {
        auto it = holdings_.find(symbol);
        if (it == holdings_.end() || it->second < quantity) {
            throw exceptions::StockTradingException("Insufficient shares to sell");
        }
        
        // In real system, we'd get current price from market data
        double estimated_price = 100.0; // placeholder
        
        if (estimated_price < min_price) {
            throw exceptions::StockTradingException("Current price below minimum price");
        }
        
        double total_proceeds = estimated_price * quantity;
        cash_balance_ += total_proceeds;
        it->second -= quantity;
        
        if (it->second == 0) {
            holdings_.erase(it);
        }
        
        return true;
    }
    
    // Portfolio statistics with constexpr if
    template<typename T = void>
    auto get_portfolio_value() const {
        double total_value = cash_balance_;
        
        for (const auto& [symbol, quantity] : holdings_) {
            // In real system, we'd multiply by current market price
            double stock_value = 100.0 * quantity; // placeholder
            total_value += stock_value;
        }
        
        if constexpr (std::is_same_v<T, std::string>) {
            return std::to_string(total_value);
        } else {
            return total_value;
        }
    }
    
    const auto& get_holdings() const noexcept { return holdings_; }
    double get_cash_balance() const noexcept { return cash_balance_; }
    void deposit_cash(double amount) noexcept { cash_balance_ += amount; }
};

// Stock market using singleton pattern
class StockMarket {
private:
    inline static StockMarket* instance_ = nullptr;
    std::unordered_map<std::string, Stock> stocks_;
    
    StockMarket() {
        // Initialize with some sample stocks
        stocks_.emplace("AAPL", Stock("AAPL", "Apple Inc.", 150.0));
        stocks_.emplace("GOOGL", Stock("GOOGL", "Alphabet Inc.", 2750.0));
        stocks_.emplace("MSFT", Stock("MSFT", "Microsoft Corporation", 300.0));
    }
    
public:
    static StockMarket& get_instance() {
        static StockMarket instance;
        return instance;
    }
    
    StockMarket(const StockMarket&) = delete;
    StockMarket& operator=(const StockMarket&) = delete;
    
    const Stock* get_stock(const std::string& symbol) const {
        auto it = stocks_.find(symbol);
        return it != stocks_.end() ? &it->second : nullptr;
    }
    
    void update_stock_price(const std::string& symbol, double new_price) {
        auto it = stocks_.find(symbol);
        if (it != stocks_.end()) {
            it->second.current_price = new_price;
            it->second.day_high = std::max(it->second.day_high, new_price);
            it->second.day_low = std::min(it->second.day_low, new_price);
        }
    }
    
    auto get_all_stocks() const {
        std::vector<std::reference_wrapper<const Stock>> result;
        for (const auto& [symbol, stock] : stocks_) {
            result.emplace_back(stock);
        }
        return result;
    }
};

} // namespace banking