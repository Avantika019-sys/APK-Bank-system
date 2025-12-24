
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef BANK_STOCK_H
#define BANK_STOCK_H
typedef boost::signals2::signal<void(std::string stockName,
                                     double UpdatedPrice)>
    StockUpdateSignal;

typedef std::vector<int> pricePerStockOverTime;
struct Stock {
  std::string name;
  pricePerStockOverTime prices;
  StockUpdateSignal signal;
};

#endif // BANK_STOCK_H
