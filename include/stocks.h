// #pragma once
// #include <string>
// #include <unordered_map>
// #include <vector>
// #include <memory>
// #include <chrono>
// #include <memory_resource>
// #include <concepts>
// #include "concepts.h"
// #include "exceptions.h"

// namespace banking {

// public:
//     StockOrder(std::string order_id, std::string symbol, Type type,
//                int quantity, double limit_price)
//         : order_id_(std::move(order_id))
//         , symbol_(std::move(symbol))
//         , type_(type)
//         , quantity_(quantity)
//         , limit_price_(limit_price)
//         , status_(Status::PENDING)
//         , timestamp_(std::chrono::system_clock::now()) {}

//     // Move semantics
//     StockOrder(StockOrder&&) = default;
//     StockOrder& operator=(StockOrder&&) = default;

//     // Getters
//     const std::string& get_order_id() const noexcept { return order_id_; }
//     const std::string& get_symbol() const noexcept { return symbol_; }
//     Type get_type() const noexcept { return type_; }
//     int get_quantity() const noexcept { return quantity_; }
//     double get_limit_price() const noexcept { return limit_price_; }
//     Status get_status() const noexcept { return status_; }

//     void execute() noexcept { status_ = Status::EXECUTED; }
//     void cancel() noexcept { status_ = Status::CANCELLED; }
//     void fail() noexcept { status_ = Status::FAILED; }
// };

// class StockPortfolio {
// private:
//     std::string portfolio_id_;
//     std::pmr::unordered_map<std::string, int> holdings_;
//     std::pmr::vector<StockOrder> order_history_;
//     double cash_balance_;

// public:
//     explicit StockPortfolio(std::string portfolio_id,
//                            std::pmr::memory_resource* pool =
//                            std::pmr::get_default_resource())
//         : portfolio_id_(std::move(portfolio_id))
//         , holdings_(pool)
//         , order_history_(pool)
//         , cash_balance_(0.0) {}

// };
