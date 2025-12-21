#include "MessageQueue.h"
#include <boost/signals2/signal.hpp>
#include <vector>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace bank::stock {
typedef boost::signals2::signal<void(std::string stockName,
                                     double UpdatedPrice)>
    StockUpdateSignal;

typedef std::string stockName;
typedef std::vector<int> pricePerStockOverTime;
struct stock {
  pricePerStockOverTime prices;
  StockUpdateSignal signal;
};
class Server {
public:
  Server();

  static Server &getInstance();

  StockUpdateSignal &getSignal(std::string stockName);
  void startSimulatingStockPriceUpdates();

  void startMessageProccesor();
  void pushMsg(Message &&msg);

private:
  double calculateStockTrend(std::string stockName);
  MessageQueue msgQueue_;
  std::map<stockName, stock> stocks_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::stock

#endif // BANK_STOCKSERVER_H
