
#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include <future>
#include <variant>

#include "transaction.h"

namespace stock {
struct order {
  order(stockTx tx);

  stockTx tx;
  std::promise<bool> prom; // result of the order
};

struct info {
  explicit info(std::string name);

  std::string stockName;
  std::promise<int> prom; // price per stock
};

typedef std::variant<order, info> variant;
struct message {
  variant data;
};
struct visitor {
  void operator()(order &bo);

  void operator()(info &so);
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
