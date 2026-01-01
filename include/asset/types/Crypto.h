#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef BANK_CRYPTO_H
#define BANK_CRYPTO_H
namespace asset::types {
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;
struct Crypto {
  Crypto(std::string name, double initPrice)
      : name_(name), priceOverTime_{initPrice}, sig_(new UpdateSignal()) {}
  Crypto(const Crypto &other) = delete;
  Crypto &operator=(const Crypto &other) = delete;

  Crypto(Crypto &&other) noexcept
      : name_(std::move(other.name_)),
        priceOverTime_(std::move(other.priceOverTime_)),
        totalCoinsOnMarket(other.totalCoinsOnMarket), sig_(other.sig_) {
    other.sig_ = nullptr;
  }
  Crypto &operator=(Crypto &&other) noexcept {
    if (this != &other) {
      delete sig_;
      sig_ = nullptr;
      std::swap(other.sig_, sig_);
      name_ = std::move(other.name_);
      priceOverTime_ = std::move(other.priceOverTime_);
      totalCoinsOnMarket = std::move(other.totalCoinsOnMarket);
    }
    return *this;
  }
  std::string name_;
  std::vector<double> priceOverTime_;
  double totalCoinsOnMarket;
  UpdateSignal *sig_;
};
} // namespace asset::types
#endif // BANK_CRYPTO_H
