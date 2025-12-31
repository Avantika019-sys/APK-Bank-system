
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"

#ifndef BANK_ASSETTRAITSCALCULATOR_H
#define BANK_ASSETTRAITSCALCULATOR_H

namespace asset::traits {
template <typename T> class Calculator;

template <> struct Calculator<types::Crypto> {
  typedef long double PrecisionT;
  static int LookBackPeriod() { return 15; }
};
template <> struct Calculator<types::Stock> {
  typedef double PrecisionT;
  static int LookBackPeriod() { return 150; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSCALCULATOR_H
