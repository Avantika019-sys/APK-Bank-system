#ifndef BANK_INFO_H
#define BANK_INFO_H
#include <future>
namespace bank::stock::messages {
struct Info {
  explicit Info(std::string name) : stockName(name) {}

  std::string stockName;
  std::promise<std::pair<int, double>> prom; // price per stock & trend
};
} // namespace bank::stock::messages
#endif // BANK_INFO_H
