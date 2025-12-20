#include "MsgQueue.h"
#include <boost/signals2/signal.hpp>
#include <vector>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace bank::stock {
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
  double calculateStockTrend(std::string stockName);
  MsgQueue msgQueue_;
  std::map<std::string, std::vector<uint>> stocks_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::stock

#endif // BANK_STOCKSERVER_H
