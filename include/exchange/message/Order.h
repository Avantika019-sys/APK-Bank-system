
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
};
struct OrderResponse {
  bool isSucceded;
};
struct OrderRequest {
  Order order;
  std::promise<OrderResponse> prom;
  std::string toString() {
    return "Asset Name: " + order.assetSymbol +
           " Manager Id:" + order.managerId +
           " Quantity:" + order.qty.toString() + " Type:" + getTypeStr();
  }
  std::string getTypeStr() {
    std::string typeStr = "BUY";
    if (order.type == OrderType::SELL) {
      typeStr = "SELL";
    }
    return typeStr;
  }
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_ORDER_H
