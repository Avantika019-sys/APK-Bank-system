#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef BANK_CRYPTO_H
#define BANK_CRYPTO_H
namespace asset::types {
struct Crypto {
  Crypto() = default;
  Crypto(std::string name, double initPrice) : name(name) {
    priceOverTime.push_back(initPrice);
  }
  std::string name;
  std::vector<int> priceOverTime;
};
} // namespace asset::types
#endif // BANK_CRYPTO_H
