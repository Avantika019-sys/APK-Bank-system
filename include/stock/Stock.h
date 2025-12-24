
#include <boost/signals2/signal.hpp>
#include <string>
typedef boost::signals2::signal<void(std::string stockName,
                                     double UpdatedPrice)>
    StockUpdateSignal;

typedef std::vector<int> pricePerStockOverTime;
struct Stock {
  pricePerStockOverTime prices;
  StockUpdateSignal signal;
};
