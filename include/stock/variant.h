
#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include "StockAccount.h"
#include <future>
#include <map>
#include <set>
#include <variant>

#include "Tx.h"

namespace bank::stock {
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

struct Info {
  explicit Info(std::string name) : stockName(name) {}

  std::string stockName;
  std::promise<std::pair<uint, double>> prom; // price per stock & trend
};
struct PortfolioTrend {
  PortfolioTrend(std::set<std::string> ownedStocks)
      : ownedStocks(ownedStocks) {}
  std::set<string> ownedStocks;
  std::promise<double> prom;
};
struct stop {};
typedef std::variant<order, Info, stop> variant;

} // namespace bank::stock

#endif // BANK_VISITOR_H
