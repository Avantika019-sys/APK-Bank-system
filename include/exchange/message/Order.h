
#include "exchange/currency/DKK.h"
#include <future>
#ifndef EXCHANGE_MESSAGE_ORDER_H
#define EXCHANGE_MESSAGE_ORDER_H
namespace exchange::message {
enum class OrderType {
  BUY,
  SELL,
};
struct OrderResponse {
  bool isSucceded;
};
struct OrderRequest {
  std::string assetName;
  std::string managerId;
  currency::DKK qty;
  OrderType type;
  std::promise<OrderResponse> prom;
  std::string toString() {
    return "Asset Name: " + assetName + " Manager Id:" + managerId +
           " Quantity:" + qty.toString() + " Type:" + getTypeStr();
  }
  std::string getTypeStr() {
    std::string typeStr = "BUY";
    if (type == OrderType::SELL) {
      typeStr = "SELL";
    }
    return typeStr;
  }
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_ORDER_H
