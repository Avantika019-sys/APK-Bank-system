#include "exchange/currency/DKK.h"
#include "exchange/util/MonitorResource.h"
#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef EXCHANGE_ASSET_CRYPTO_H
#define EXCHANGE_ASSET_CRYPTO_H
namespace exchange::asset {
typedef boost::signals2::signal<void(currency::DKK UpdatedPrice)> UpdateSignal;
class Crypto {
public:
  Crypto(std::string name, util::MonitorResource *s);
  Crypto(const Crypto &other) = delete;
  Crypto &operator=(const Crypto &other) = delete;

  Crypto(Crypto &&other) noexcept;
  Crypto &operator=(Crypto &&other) noexcept;
  ~Crypto();

  std::string name_;
  std::pmr::vector<currency::DKK> unitPriceOverTime_;
  double totalCoinsOnMarket;
  UpdateSignal *sig_;
};
} // namespace exchange::asset
#endif // EXCHANGE_ASSET_CRYPTO_H
