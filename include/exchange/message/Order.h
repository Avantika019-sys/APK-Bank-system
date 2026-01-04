
#include "exchange/currency/DKK.h"
#include <future>
#ifndef EXCHANGE_MESSAGE_ORDER_H
#define EXCHANGE_MESSAGE_ORDER_H
namespace exchange::message {
enum class OrderType {
  BUY,
  SELL,
};
struct Order {
  std::string assetSymbol;
  std::string managerId;
  currency::DKK qty;
  OrderType type;
  std::string toString() const {
    std::string typeStr = "BUY";
    if (type == OrderType::SELL) {
      typeStr = "SELL";
    }
    return "Asset Name: " + assetSymbol + " Manager Id:" + managerId +
           " Quantity:" + qty.toString() + " Type:" + typeStr;
  }
};
struct OrderResponse {
  bool isSucceded;
};
struct OrderRequest {
  Order order;
  std::promise<OrderResponse> prom;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_ORDER_H
