#include "account.h"
#include "stock/server.h"

class StockAccount : public Account {
public:
  StockAccount(std::string name, std::string id);
  void buyStock(uint amountOfStocks, std::string stockName);
  void sellStock(uint amount, std::string name);
  void setMonitorStocks(bool monitor);
  StockAccount(StockAccount &&other);
  StockAccount &operator=(StockAccount &&other);

  StockAccount(const StockAccount &) = delete;
  StockAccount &operator=(const StockAccount &) = delete;

private:
  void onStockUpdate(std::string stockName, int updatedPrice);

  bool monitorStocks;
  std::map<std::string, int> ownedStocks_;
  std::mutex mtx_;
};

//    template<typename... Args>
// void place_buy_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
// }
//
// template <typename... Args> void place_sell_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
// }
