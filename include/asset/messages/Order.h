
#include "asset/traits/Print.h"
#include <future>
#ifndef BANK_ORDER_H
#define BANK_ORDER_H
namespace asset::messages {
enum class OrderType {
  BUY,
  SELL,
};
struct OrderResponse {
  bool isSucceded;
};
template <typename T> struct OrderRequest {
  std::string assetName;
  double amountOfAsset;
  OrderType type;
  std::promise<OrderResponse> prom;
  std::string toString() {
    std::string typeStr = "BUY";
    if (type == OrderType::SELL) {
      typeStr = "SELL";
    }
    return traits::Print<T>::Header() + ": " + assetName + "\n" +
           "quantity: " + amountOfAsset + "\n" + "type:" + typeStr;
  }
};
} // namespace asset::messages
#endif // BANK_ORDER_H
