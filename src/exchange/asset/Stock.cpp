#include "exchange/asset/Stock.h"
namespace exchange::asset {
Stock::Stock(std::string name, std::string symbol,
             util::observability::MonitorResource &s, int openHour,
             int closeHour)
    : name_(name), symbol(symbol), unitPriceOverTime_(&s),
      sig_(new UpdateSignal()), openHour(openHour), closeHour(closeHour) {}

Stock::Stock(Stock &&other) noexcept
    : name_(std::move(other.name_)), symbol(other.symbol),
      unitPriceOverTime_(std::move(other.unitPriceOverTime_)),
      sig_(other.sig_) {
  other.sig_ = nullptr;
}

currency::DKK Stock::getLatestPrice() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribInt(0, 23);
  std::uniform_real_distribution<double> distribDouble(-0.005, 0.005);

  int randomHour = distribInt(gen);

  if (randomHour < closeHour && randomHour > openHour) {
    double percentChange = distribDouble(gen);
    unitPriceOverTime_.emplace_back(unitPriceOverTime_.back().value() *
                                    (1 + percentChange));
  } else {
    unitPriceOverTime_.push_back(unitPriceOverTime_.back());
  }
  return unitPriceOverTime_.back();
}
Stock &Stock::operator=(Stock &&other) noexcept {
  if (this != &other) {
    delete sig_;
    sig_ = nullptr;
    std::swap(other.sig_, sig_);
    name_ = std::move(other.name_);
    unitPriceOverTime_ = std::move(other.unitPriceOverTime_);
    symbol = std::move(other.symbol);
  }
  return *this;
}
Stock::~Stock() { delete sig_; }
} // namespace exchange::asset
