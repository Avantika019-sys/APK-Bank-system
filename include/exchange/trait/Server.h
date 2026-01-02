#include "exchange/asset.hpp"

#ifndef EXCHANGE_ASSETTRAITSSERVER_H
#define EXCHANGE_ASSETTRAITSSERVER_H

namespace exchange::trait {
template <typename T> class Server;

template <> struct Server<asset::Crypto> {
  static int UpdateRate() { return 1; }
};
template <> struct Server<asset::Stock> {
  static int UpdateRate() { return 5; }
};
} // namespace exchange::trait
#endif // EXCHANGE_ASSETTRAITSSERVER_H
