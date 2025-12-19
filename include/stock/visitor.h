
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
  std::promise<bool> prom; // result of the order
};

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

#endif // BANK_VISITOR_H
