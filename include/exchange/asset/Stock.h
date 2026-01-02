
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef EXCHANGE_ASSET_STOCK_H
#define EXCHANGE_ASSET_STOCK_H
namespace exchange::asset {
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;
struct Stock {
  Stock(std::string name, double initPrice)
      : name_(name), unitPriceOverTime_{initPrice}, sig_(new UpdateSignal()) {}

  Stock(const Stock &other) = delete;
  Stock &operator=(const Stock &other) = delete;

  Stock(Stock &&other) noexcept
      : name_(std::move(other.name_)),
        unitPriceOverTime_(std::move(other.unitPriceOverTime_)), sig_(other.sig_) {
    other.sig_ = nullptr;
  }
  Stock &operator=(Stock &&other) noexcept {
    if (this != &other) {
      delete sig_;
      sig_ = nullptr;
      std::swap(other.sig_, sig_);
      name_ = std::move(other.name_);
      unitPriceOverTime_ = std::move(other.unitPriceOverTime_);
    }
    return *this;
  }
  ~Stock() { delete sig_; }
  std::string name_;
  std::vector<double> unitPriceOverTime_;
  UpdateSignal *sig_;
};
} // namespace exchange::asset

#endif // EXCHANGE_ASSET_STOCK_H
