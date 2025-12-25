
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef BANK_STOCK_H
#define BANK_STOCK_H
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;

struct Stock {
  std::string name;
  std::vector<int> priceOverTime;
  UpdateSignal signal;
};

#endif // BANK_STOCK_H
