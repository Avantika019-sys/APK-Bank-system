

#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"

#ifndef BANK_ASSETTRAITSPRECISION_H
#define BANK_ASSETTRAITSPRECISION_H

namespace asset::traits {
template <typename T> class Precision;

template <> struct Precision<types::Crypto> {
  typedef long double PrecisionT;
  static int PrecisionFormat() { return 20; }
};
template <> struct Precision<types::Stock> {
  typedef double PrecisionT;
  static int PrecisionFormat() { return 15; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSPRECISION_H
