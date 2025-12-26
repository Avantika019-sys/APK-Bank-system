#ifndef BANK_INFO_H
#define BANK_INFO_H
#include <future>
namespace asset::messages {
template <typename T> struct InfoResponse {
  InfoResponse(int currentPrice, double trend)
      : currentPrice(currentPrice), trend(trend) {}
  int currentPrice;
  double trend;
};
template <typename T> struct InfoRequest {
  explicit InfoRequest(std::string name) : assetName(name) {}

  std::string assetName;
  std::promise<InfoResponse<T>> prom;
};
} // namespace asset::messages
#endif // BANK_INFO_H
