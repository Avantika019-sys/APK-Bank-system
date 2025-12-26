#include "types/Crypto.h"
#include "types/Stock.h"

#ifndef BANK_ASSETTRAITS_H
#define BANK_ASSETTRAITS_H

namespace asset {
template <typename T> class Traits;

template <> struct Traits<types::Crypto> {
  typedef long double AccT;
  static int updateRate() { return 1; }
  static int LookBackPeriod() { return 15; }
};
template <> struct Traits<types::Stock> {
  typedef double AccT;
  static int updateRate() { return 5; }
  static int LookBackPeriod() { return 150; }
};
} // namespace asset
#endif // BANK_ASSETTRAITS_H
