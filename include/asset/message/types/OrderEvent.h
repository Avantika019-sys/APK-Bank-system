
#include <functional>
#include <string>
#ifndef BANK_ORDEREVENT_H
#define BANK_ORDEREVENT_H
namespace asset::message::types {
template <typename T> struct OrderEvent {
  std::function<void(std::string assetName, int qty, int totalNoOfAssetForSale,
                     int totalNoOfAssetDemand, double price, bool isBuy)>
      cb;
};
} // namespace asset::message::types
#endif // BANK_ORDEREVENT_H
