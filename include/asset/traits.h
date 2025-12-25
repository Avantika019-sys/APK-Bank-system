#include "Crypto.h"
#include "Stock.h"

#ifndef BANK_ASSETTRAITS_H
#define BANK_ASSETTRAITS_H
template <typename T> class AssetTraits;

template <> struct AssetTraits<Crypto> {
  typedef long double AccT;
  static int updateRate() { return 1; }
  static int LookBackPeriod() { return 15; }
};
template <> struct AssetTraits<Stock> {
  typedef double AccT;
  static int updateRate() { return 5; }
  static int LookBackPeriod() { return 150; }
};
#endif // BANK_ASSETTRAITS_H
