#ifndef BANK_ORDER_H
#define BANK_ORDER_H

#include <future>
namespace bank::stock::messages {
enum class OrderType {
  BUY,
  SELL,
};
struct OrderResponse {
  bool isSucceded;
};
struct OrderRequest {
  OrderRequest(std::string stockName, int amountOfStocks, OrderType type)
      : stockName(stockName), amountOfStocks(amountOfStocks), type(type) {}
  std::string stockName;
  int amountOfStocks;
  OrderType type;
  std::promise<OrderResponse> prom;
};
} // namespace bank::stock::messages
#endif // BANK_ORDER_H
