#include "Crypto.h"
#include "Stock.h"
template <typename T> class AssetTraits;

template <> struct AssetTraits<Crypto> {
  typedef long double AccT;
  static int updateRate() { return 1; }
  static int lookbackPeriod() { return 15; }
};
template <> struct AssetTraits<Stock> {
  typedef double AccT;
  static int updateRate() { return 5; }
  static int lookbackPeriod() { return 150; }
};
