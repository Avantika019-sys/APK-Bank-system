

#include "exchange/asset.hpp"

#ifndef EXCHANGE_TRAIT_PRECISION_H
#define EXCHANGE_TRAIT_PRECISION_H

namespace exchange::trait {
template <typename T> class Precision;

template <> struct Precision<asset::Crypto> {
  typedef long double PrecisionT;
  static int PrecisionFormat() { return 20; }
};
template <> struct Precision<asset::Stock> {
  typedef double PrecisionT;
  static int PrecisionFormat() { return 15; }
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_PRECISION_H
