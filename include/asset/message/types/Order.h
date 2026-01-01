
#include "asset/traits/Print.h"
#include <future>
#ifndef BANK_ORDER_H
#define BANK_ORDER_H
namespace asset::message::types {
enum class OrderType {
  BUY,
  SELL,
};
struct OrderResponse {
  bool isSucceded;
};
template <typename T> struct OrderRequest {
  std::string assetName;
  std::string managerId;
  double qty;
  OrderType type;
  std::promise<OrderResponse> prom;
  std::string getTypeStr() {
    std::string typeStr = "BUY";
    if (type == OrderType::SELL) {
      typeStr = "SELL";
    }
    return typeStr;
  }
};
} // namespace asset::message::types
#endif // BANK_ORDER_H
