#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#ifndef BANK_CRYPTO_H
#define BANK_CRYPTO_H
struct Crypto {
  std::string name;
  std::vector<int> priceOverTime;
};
#endif // BANK_CRYPTO_H
