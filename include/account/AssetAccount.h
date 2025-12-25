#include "account.h"
#include "stock/Server.h"
#include <boost/signals2/connection.hpp>
#include <map>

namespace bank::account {
struct ownedStock {
  int NoOfStocksOwned;
  std::optional<int> stopLossRule;
  boost::signals2::connection conn;
};
class AssetAccount : public Account {
public:
  AssetAccount(std::string name, std::string id);
  void buyStock(std::string name, int qty);
  void sellStock(std::string name, int qty);
  void printPortfolio();
  void addStopLossRule(std::string name, int limit);
  void removeStopLossRule(std::string name, int limit);
  AssetAccount(AssetAccount &&other) noexcept;
  AssetAccount &operator=(AssetAccount &&other) noexcept;

  AssetAccount(const AssetAccount &) = delete;
  AssetAccount &operator=(const AssetAccount &) = delete;

private:
  void onStockUpdate(std::string stockName, int updatedPrice);
  std::map<std::string, ownedStock> portfolio_;
  std::mutex mtx_;
};

//    template<typename... Args>
// void place_buy_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
// }
//
// template <typename... Args> void place_sell_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
} // namespace bank::account
