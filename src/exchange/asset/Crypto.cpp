#include "exchange/asset/Crypto.h"
#include <random>
namespace exchange::asset {
Crypto::Crypto(std::string name, std::string symbol,
               util::observability::MonitorResource &s)
    : name_(name), symbol(symbol), sig_(new UpdateSignal()),
      unitPriceOverTime_(&s) {}

Crypto::Crypto(Crypto &&other) noexcept
    : name_(std::move(other.name_)),
      unitPriceOverTime_(std::move(other.unitPriceOverTime_)),
      totalCoinsOnMarket(std::move(other.totalCoinsOnMarket)),
      symbol(std::move(other.symbol)), sig_(other.sig_) {
  other.sig_ = nullptr;
}
currency::DKK Crypto::getLatestPrice() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribInt(0, 23);
  std::uniform_real_distribution<double> distribDouble(-0.005, 0.005);

  double percentChange = distribDouble(gen);
  unitPriceOverTime_.emplace_back(unitPriceOverTime_.back().value() *
                                  (1 + percentChange));
  return unitPriceOverTime_.back();
}
Crypto &Crypto::operator=(Crypto &&other) noexcept {
  if (this != &other) {
    delete sig_;
    sig_ = nullptr;
    std::swap(other.sig_, sig_);
    name_ = std::move(other.name_);
    unitPriceOverTime_ = std::move(other.unitPriceOverTime_);
    totalCoinsOnMarket = std::move(other.totalCoinsOnMarket);
    symbol = std::move(other.symbol);
  }
  return *this;
}
Crypto::~Crypto() { delete sig_; }
} // namespace exchange::asset
