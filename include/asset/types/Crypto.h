#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef BANK_CRYPTO_H
#define BANK_CRYPTO_H
namespace asset::types {
struct Crypto {
  std::string name;
  std::vector<double> priceOverTime;
  double totalCoins;
};
} // namespace asset::types
#endif // BANK_CRYPTO_H
