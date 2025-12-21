#include "MessageQueue.h"
#include <boost/signals2/signal.hpp>
#include <vector>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace bank::stock {
typedef boost::signals2::signal<void(std::string stockName,
                                     double UpdatedPrice)>
    StockUpdateSignal;
class Server {
public:
  Server();

  static Server &getInstance();

  // boost::signals2::signal<void(std::string stockName, double UpdatedPrice)>
  // sig;
  StockUpdateSignal &getSignal(std::string stockName);
  void startUpdateStocksWorker();

  void startStockWorker();
  void pushMsg(Message &&msg);

private:
  double calculateStockTrend(std::string stockName);
  MessageQueue msgQueue_;
  std::map<std::string, std::pair<std::vector<uint>, StockUpdateSignal>>
      stocks_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::stock

#endif // BANK_STOCKSERVER_H
