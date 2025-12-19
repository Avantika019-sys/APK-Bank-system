#include "MsgQueue.h"
#include <boost/signals2/signal.hpp>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace stock {
class Server {
public:
  Server();

  static Server &getInstance();

  boost::signals2::signal<void(std::string stockName, double UpdatedPrice)> sig;

  void startUpdateStocksWorker();

  void startStockWorker();
  void pushMsg(variant &&msg);
  void stopWorkers();

private:
  MsgQueue msgQueue_;
  std::vector<std::pair<std::string, int>> stocks;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace stock

#endif // BANK_STOCKSERVER_H
