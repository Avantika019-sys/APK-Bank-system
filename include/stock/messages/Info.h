#ifndef BANK_INFO_H
#define BANK_INFO_H
#include <future>
namespace bank::stock::messages {
struct InfoResponse {
  InfoResponse(int currentPrice, double trend)
      : currentPrice(currentPrice), trend(trend) {}
  int currentPrice;
  double trend;
};
struct InfoRequest {
  explicit InfoRequest(std::string name) : stockName(name) {}

  std::string stockName;
  std::promise<InfoResponse> prom;
};
} // namespace bank::stock::messages
#endif // BANK_INFO_H
