
#ifndef EXCHANGE_ORDERARRAY_H
#define EXCHANGE_ORDERARRAY_H
#include "exchange/currency/DKK.h"
#include "message/Order.h"
#include <string>
using namespace exchange::message;
namespace exchange {
class OrderArray {
public:
  OrderArray(int size) : size_(size), currentOrders_(0) {
    orders_ = new Order[size];
  }
  ~OrderArray() { delete[] orders_; }
  OrderArray(const OrderArray &other)
      : size_(other.size_), currentOrders_(other.currentOrders_) {
    orders_ = new Order[other.size_];
    for (int i = 0; i < size_; i++) {
      orders_[i] = other.orders_[i];
    }
  }
  void addOrder(const Order &order) {
    if (currentOrders_ == size_) {
      Order *newOrders = new Order[size_ + 10];
      for (int i = 0; i < size_; i++) {
        newOrders[i] = orders_[i];
      }
      size_ = size_ + 10;
      delete[] orders_;
      orders_ = newOrders;
    }
    orders_[currentOrders_++] = order;
  }
  OrderArray &operator=(const OrderArray &other) {
    OrderArray temp(other);
    Swap(temp);
    return *this;
  }
  void Swap(OrderArray &other) {
    std::swap(orders_, other.orders_);
    std::swap(size_, other.size_);
    std::swap(currentOrders_, other.currentOrders_);
  }
  int size_;
  int currentOrders_;
  Order *orders_;
};
} // namespace exchange
#endif // EXCHANGE_ORDERARRAY_H
