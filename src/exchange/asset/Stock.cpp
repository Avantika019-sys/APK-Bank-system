#include "exchange/asset/Stock.h"
namespace exchange::asset {
Stock::Stock(std::string name, util::MonitorResource *s)
    : name_(name), unitPriceOverTime_(s), sig_(new UpdateSignal()) {}

Stock::Stock(Stock &&other) noexcept
    : name_(std::move(other.name_)),
      unitPriceOverTime_(std::move(other.unitPriceOverTime_)),
      sig_(other.sig_) {
  other.sig_ = nullptr;
}
Stock &Stock::operator=(Stock &&other) noexcept {
  if (this != &other) {
    delete sig_;
    sig_ = nullptr;
    std::swap(other.sig_, sig_);
    name_ = std::move(other.name_);
    unitPriceOverTime_ = std::move(other.unitPriceOverTime_);
  }
  return *this;
}
Stock::~Stock() { delete sig_; }
} // namespace exchange::asset
