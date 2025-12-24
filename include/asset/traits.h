#include "Crypto.h"
#include "Stock.h"
template <typename T> class AssetTraits;

template <> struct AssetTraits<Crypto> {
  static int defaultWindow() { return 100; }
  typedef long double AccT;
  static int updateRate() { return 1; }
  static int lookbackPeriod() { return 15; }
};
template <> struct AssetTraits<Stock> {
  static int defaultWindow() { return 50; }
  typedef double AccT;
  static int updateRate() { return 5; }
  static int lookbackPeriod() { return 150; }
};
