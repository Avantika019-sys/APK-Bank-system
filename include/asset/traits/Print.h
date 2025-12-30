
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"

#ifndef BANK_ASSETTRAITSPRINT_H
#define BANK_ASSETTRAITSPRINT_H

namespace asset::traits {
template <typename T> class Print;

template <> struct Print<types::Crypto> {
  static std::string Header() { return "Crypto"; }
};
template <> struct Print<types::Stock> {
  static std::string Header() { return "Stock"; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSPRINT_H
