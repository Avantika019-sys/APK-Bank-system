#ifndef BANK_INFO_H
#define BANK_INFO_H
#include "asset/traits/Server.h"
#include <future>
namespace asset::message::types {
template <typename T> struct InfoResponse {
  double currentPrice;
  double trend;
};
template <typename T> struct InfoRequest {
  std::string assetName;
  std::promise<InfoResponse<T>> prom;
};
} // namespace asset::message::types
#endif // BANK_INFO_H
