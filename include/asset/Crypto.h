#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef BANK_CRYPTO_H
#define BANK_CRYPTO_H
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;
struct Crypto {
  std::string name;
  std::vector<int> priceOverTime;
  UpdateSignal signal;
};
#endif // BANK_CRYPTO_H
