#ifndef EXCHANGE_MESSAGE_INFO_H
#define EXCHANGE_MESSAGE_INFO_H
#include <future>
#include <string>
#include <vector>
namespace exchange::message {
struct assetInfo {
  std::string symbol;
  double currentPrice;
  double trend;
};
struct InfoResponse {
  std::vector<assetInfo> assetInfos;
};
struct InfoRequest {
  std::vector<std::string> assetSymbols;
  std::promise<InfoResponse> prom;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_INFO_H
