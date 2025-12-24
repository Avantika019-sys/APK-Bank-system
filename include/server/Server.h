#include "./../asset/Crypto.h"
#include "./../asset/Stock.h"
#include "MessageQueue.h"
#include <boost/signals2/signal.hpp>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H
namespace bank::server {
class Server {
public:
  Server();

  static Server &getInstance();

  StockUpdateSignal &getSignal(std::string stockName);
  void startSimulatingStockPriceUpdates();

  void startMessageProccesor();
  void pushMsg(Message &&msg);

private:
  MessageQueue msgQueue_;
  std::map<std::string, Stock> stocks_;
  std::map<std::string, Crypto> crypto_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::server

#endif // BANK_STOCKSERVER_H
