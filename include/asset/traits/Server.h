#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"

#ifndef BANK_ASSETTRAITSSERVER_H
#define BANK_ASSETTRAITSSERVER_H

namespace asset::traits {
template <typename T> class Server;

template <> struct Server<types::Crypto> {
  static int UpdateRate() { return 1; }
};
template <> struct Server<types::Stock> {
  static int UpdateRate() { return 5; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSSERVER_H
