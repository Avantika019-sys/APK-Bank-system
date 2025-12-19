#include <boost/signals2/signal.hpp>
#include <future>
#include <set>

#include "messageQueue.h"
#include "transaction.h"
#include "visitor.h"
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
  // delete copy and assignment ctor
private:
  std::vector<std::pair<std::string, int>> stocks;
  visitor visitor_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace stock

// auto get_all_stocks() const {
//     std::vector<std::reference_wrapper<const Stock>> result;
//     for (const auto& [symbol, stock] : stocks_) {
//         result.emplace_back(stock);
//     }
//     return result;
// }

// Portfolio statistics with constexpr if
// template<typename T = void>
// auto get_portfolio_value() const {
//     double total_value = cash_balance_;

//     for (const auto& [symbol, quantity] : holdings_) {
//         // In real system, we'd multiply by current market price
//         double stock_value = 100.0 * quantity; // placeholder
//         total_value += stock_value;
//     }

//     if constexpr (std::is_same_v<T, std::string>) {
//         return std::to_string(total_value);
//     } else {
//         return total_value;
//     }
// }

#endif // BANK_STOCKSERVER_H
