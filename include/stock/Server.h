#include "MessageQueue.h"
#include "Stock.h"
#include <boost/signals2/signal.hpp>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace bank::stock {
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
  std::map<std::string, StockData> stocks_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::stock

#endif // BANK_STOCKSERVER_H
