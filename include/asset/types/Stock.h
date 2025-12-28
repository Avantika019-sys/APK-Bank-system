
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef BANK_STOCK_H
#define BANK_STOCK_H
namespace asset::types {
struct Stock {
  Stock() = default;
  Stock(std::string name, double initPrice) : name(name) {
    priceOverTime.push_back(initPrice);
  }
  std::string name;
  std::vector<int> priceOverTime;
};
} // namespace asset::types

#endif // BANK_STOCK_H
