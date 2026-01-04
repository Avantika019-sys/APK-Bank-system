#include "exchange/asset/Crypto.h"
namespace exchange::asset {
Crypto::Crypto(std::string name, std::string symbol,
               util::observability::MonitorResource *s)
    : name_(name), symbol(symbol), sig_(new UpdateSignal()),
      unitPriceOverTime_(s) {}

Crypto::Crypto(Crypto &&other) noexcept
    : name_(std::move(other.name_)),
      unitPriceOverTime_(std::move(other.unitPriceOverTime_)),
      totalCoinsOnMarket(std::move(other.totalCoinsOnMarket)),
      symbol(std::move(other.symbol)), sig_(other.sig_) {
  other.sig_ = nullptr;
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
