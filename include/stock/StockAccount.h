#include "account.h"
#include "stock/Server.h"

namespace bank::stock {
class StockAccount : public Account {
public:
  StockAccount(std::string name, std::string id);
  void buyStock(std::string name, uint qty);
  void sellStock(std::string name, uint qty);
  void setMonitorStocks(bool monitor);
  void printPortfolio();
  void addStopLossRule(std::string name, uint limit);
  void removeStopLossRule(std::string name, uint limit);
  StockAccount(StockAccount &&other) noexcept;
  StockAccount &operator=(StockAccount &&other) noexcept;

  StockAccount(const StockAccount &) = delete;
  StockAccount &operator=(const StockAccount &) = delete;

private:
  void onStockUpdate(std::string stockName, uint updatedPrice);
  std::map<std::string, std::pair<uint, std::optional<uint>>> portfolio_;
  std::mutex mtx_;
};

//    template<typename... Args>
// void place_buy_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
// }
//
// template <typename... Args> void place_sell_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
} // namespace bank::stock
