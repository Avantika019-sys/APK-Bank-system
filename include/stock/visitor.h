
#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include <future>
#include <variant>

#include "transaction.h"

namespace stock {
enum class orderType {
  BUY,
  SELL,
};
struct order {
  order(string stockName, int amountOfStocks, orderType type);
  std::string stockName;
  int amountOfStocks;
  orderType type;
  // stockTx tx;
  std::promise<bool> prom; // result of the order
};
// class StockOrder {
// public:

// private:
//     std::string order_id_;
//     std::string symbol_;
//     Type type_;
//     int quantity_;
//     double limit_price_;
//     Status status_;
//     std::chrono::system_clock::time_point timestamp_;

struct info {
  explicit info(std::string name);

  std::string stockName;
  std::promise<int> prom; // price per stock
};
struct stop {};
typedef std::variant<order, info, stop> variant;

struct visitor {
  void operator()(order &o);
  void operator()(info &i);
  void operator()(stop &s);
};
} // namespace stock

struct Stock {
  std::string symbol;
  std::string name;
  double current_price;
  double day_high;
  double day_low;

  Stock(std::string sym, std::string n, double price)
      : symbol(std::move(sym)), name(std::move(n)), current_price(price),
        day_high(price), day_low(price) {}
};

#endif // BANK_VISITOR_H
