

#include <map>
#include <string>
#ifndef BANK_CRYPTOBOT_H
#define BANK_CRYPTOBOT_H
namespace asset::bots {
class Crypto {
public:
  void OnNewOrder(std::string assetName, int qty, int totalNoOfAssetForSale,
                  int totalNoOfAssetDemand, double price, bool isBuy) {
    popularOrders[assetName] = qty;
  }

private:
  std::map<std::string, double> popularOrders;
};
} // namespace asset::bots
#endif // BANK_CRYPTOBOT_H
