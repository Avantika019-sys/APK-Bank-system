
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
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_ORDER_H
