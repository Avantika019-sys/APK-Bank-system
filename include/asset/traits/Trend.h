
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"

#ifndef BANK_ASSETTRAITSCALCULATOR_H
#define BANK_ASSETTRAITSCALCULATOR_H

namespace asset::traits {
template <typename T> class Trend;

template <> struct Trend<types::Crypto> {
  static int LookBackPeriod() { return 15; }
};
template <> struct Trend<types::Stock> {
  static int LookBackPeriod() { return 150; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSCALCULATOR_H
