
#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include <future>
#include <variant>

#include "Tx.h"

namespace stock {
enum class orderType {
  BUY,
  SELL,
};
struct order {
  order(string stockName, int amountOfStocks, orderType type)
      : stockName(stockName), amountOfStocks(amountOfStocks), type(type) {}
  std::string stockName;
  int amountOfStocks;
  orderType type;
  std::promise<bool> prom; // result of the order
};

struct info {
  explicit info(std::string name) : stockName(name) {}

  std::string stockName;
  std::promise<int> prom; // price per stock
};
struct stop {};
typedef std::variant<order, info, stop> variant;

} // namespace stock

#endif // BANK_VISITOR_H
