
#include <functional>
#include <string>
#ifndef BANK_ORDEREVENT_H
#define BANK_ORDEREVENT_H
namespace asset::messages {
template <typename T> struct OrderEvent {
  std::function<void(std::string assetName, int qty, int totalNoOfAssetForSale,
                     int totalNoOfAssetDemand, double price, bool isBuy)>
      cb;
};
} // namespace asset::messages
#endif // BANK_ORDEREVENT_H
