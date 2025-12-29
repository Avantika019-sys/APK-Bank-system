#ifndef BANK_INFO_H
#define BANK_INFO_H
#include "asset/traits/Calculator.h"
#include "asset/traits/Server.h"
#include <future>
namespace asset::messages {
template <typename T> struct InfoResponse {
  int currentPrice;
  traits::Calculator<T>::AccT trend;
};
template <typename T> struct InfoRequest {
  std::string assetName;
  std::promise<InfoResponse<T>> prom;
};
} // namespace asset::messages
#endif // BANK_INFO_H
