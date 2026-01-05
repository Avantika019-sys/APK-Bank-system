

#include "exchange/asset.hpp"

#ifndef EXCHANGE_TRAIT_PRECISION_H
#define EXCHANGE_TRAIT_PRECISION_H

namespace exchange::trait {
template <typename T> class Precision;

template <> struct Precision<asset::Crypto> {
  typedef long double PrecisionT;
};
template <> struct Precision<asset::Stock> {
  typedef double PrecisionT;
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_PRECISION_H
