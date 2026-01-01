
#include "asset/types/Crypto.h"
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef BANK_STOCK_H
#define BANK_STOCK_H
namespace asset::types {
struct Stock {
  Stock(std::string name, double initPrice)
      : name_(name), priceOverTime_{initPrice}, sig_(new UpdateSignal()) {}

  Stock(const Stock &other) = delete;
  Stock &operator=(const Stock &other) = delete;

  Stock(Stock &&other) noexcept
      : name_(std::move(other.name_)),
        priceOverTime_(std::move(other.priceOverTime_)), sig_(other.sig_) {
    other.sig_ = nullptr;
  }
  Stock &operator=(Stock &&other) noexcept {
    if (this != &other) {
      delete sig_;
      sig_ = nullptr;
      std::swap(other.sig_, sig_);
      name_ = std::move(other.name_);
      priceOverTime_ = std::move(other.priceOverTime_);
    }
    return *this;
  }
  ~Stock() { delete sig_; }
  std::string name_;
  std::vector<double> priceOverTime_;
  UpdateSignal *sig_;
};
} // namespace asset::types

#endif // BANK_STOCK_H
