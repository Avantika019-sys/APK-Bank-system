
#include "exchange/asset.hpp"

#ifndef EXCHANGE_TRAIT_TREND_H
#define EXCHANGE_TRAIT_TREND_H

namespace exchange::trait {
template <typename T> class Trend;

template <> struct Trend<asset::Crypto> {
  static int LookBackPeriod() { return 80; }
};
template <> struct Trend<asset::Stock> {
  static int LookBackPeriod() { return 150; }
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_TREND_H
