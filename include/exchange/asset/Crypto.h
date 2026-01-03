#include "exchange/MonitorResource.h"
#include "exchange/currency/DKK.h"
#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef EXCHANGE_ASSET_CRYPTO_H
#define EXCHANGE_ASSET_CRYPTO_H
namespace exchange::asset {
typedef boost::signals2::signal<void(std::string assetName,
                                     currency::DKK UpdatedPrice)>
    UpdateSignal;
struct Crypto {
  Crypto(std::string name, MonitorResource *s)
      : name_(name), sig_(new UpdateSignal()), unitPriceOverTime_(s) {}
  Crypto(const Crypto &other) = delete;
  Crypto &operator=(const Crypto &other) = delete;

  Crypto(Crypto &&other) noexcept
      : name_(std::move(other.name_)),
        unitPriceOverTime_(std::move(other.unitPriceOverTime_)),
        totalCoinsOnMarket(std::move(other.totalCoinsOnMarket)),
        sig_(other.sig_) {
    other.sig_ = nullptr;
  }
  Crypto &operator=(Crypto &&other) noexcept {
    if (this != &other) {
      delete sig_;
      sig_ = nullptr;
      std::swap(other.sig_, sig_);
      name_ = std::move(other.name_);
      unitPriceOverTime_ = std::move(other.unitPriceOverTime_);
      totalCoinsOnMarket = std::move(other.totalCoinsOnMarket);
    }
    return *this;
  }
  ~Crypto() { delete sig_; }
  std::string name_;
  std::pmr::vector<currency::DKK> unitPriceOverTime_;
  double totalCoinsOnMarket;
  UpdateSignal *sig_;
};
} // namespace exchange::asset
#endif // EXCHANGE_ASSET_CRYPTO_H
