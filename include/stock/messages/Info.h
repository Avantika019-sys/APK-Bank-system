#ifndef BANK_INFO_H
#define BANK_INFO_H
#include <future>
namespace bank::stock::messages {
struct InfoResponse {
  int latestPrice;
  double trend;
};
struct InfoRequest {
  explicit InfoRequest(std::string name) : stockName(name) {}

  std::string stockName;
  std::promise<InfoResponse> prom; // price per stock & trend
};
} // namespace bank::stock::messages
#endif // BANK_INFO_H
