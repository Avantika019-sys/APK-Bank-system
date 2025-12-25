
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef BANK_STOCK_H
#define BANK_STOCK_H
struct Stock {
  std::string name;
  std::vector<int> priceOverTime;
};

#endif // BANK_STOCK_H
