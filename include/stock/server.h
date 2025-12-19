#include "messageQueue.h"
#include "visitor.h"
#include <boost/signals2/signal.hpp>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace stock {
class server {
public:
  server();

  static server &getInstance();

  boost::signals2::signal<void(std::string stockName, double UpdatedPrice)> sig;

  void startUpdateStocksWorker();

  void startStockWorker();

  messageQueue msgQueue;

  void stopWorkers();

private:
  std::vector<std::pair<std::string, int>> stocks;
  visitor visitor_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace stock

#endif // BANK_STOCKSERVER_H
