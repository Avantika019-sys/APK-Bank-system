
#include "exchange/asset.hpp"

#ifndef EXCHANGE_TRAIT_PRINT_H
#define EXCHANGE_TRAIT_PRINT_H

namespace exchange::trait {
template <typename T> class Print;

template <> struct Print<asset::Crypto> {
  static std::string Header() { return "Crypto"; }
};
template <> struct Print<asset::Stock> {
  static std::string Header() { return "Stock"; }
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_PRINT_H
