#ifndef BANK_ORDER_H
#define BANK_ORDER_H

#include <future>
namespace bank::server::messages {
enum class OrderType {
  BUY,
  SELL,
};
struct OrderResponse {
  bool isSucceded;
};
template <typename T> struct OrderRequest {
  OrderRequest(std::string assetName, int amountOfAsset, OrderType type)
      : assetName(assetName), amountOfAsset(amountOfAsset), type(type) {}
  std::string assetName;
  int amountOfAsset;
  OrderType type;
  std::promise<OrderResponse> prom;
};
} // namespace bank::server::messages
#endif // BANK_ORDER_H
