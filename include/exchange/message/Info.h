#ifndef EXCHANGE_MESSAGE_INFO_H
#define EXCHANGE_MESSAGE_INFO_H
#include <future>
#include <string>
namespace exchange::message {
template <typename T> struct InfoResponse {
  double currentPrice;
  double trend;
};
template <typename T> struct InfoRequest {
  std::string assetName;
  std::promise<InfoResponse<T>> prom;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_INFO_H
