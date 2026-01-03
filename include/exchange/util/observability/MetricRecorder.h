

#include "Field.h"
#include <map>
#include <mutex>
#include <string>
#include <vector>
#ifndef EXCHANGE_UTIL_OBSERVABILITY_METRICRECORDER_H
#define EXCHANGE_UTIL_OBSERVABILITY_METRICRECORDER_H
namespace exchange::util::observability {
struct MetricDatapoint {
  int count;
  std::vector<std::string> tags;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
};
class MetricRecorder {
public:
  template <typename... Ts>
  void incrementMetric(std::string name, int amount, field<Ts>... fields) {
    std::lock_guard<std::mutex> lock(mtx_);
    MetricDatapoint m{amount, {getFieldValueStr(fields)...}};
    metrics[name].push_back(m);
  }

private:
  template <typename T> std::string getFieldValueStr(field<T> field) {
    std::string fieldValStr = "";
    // if constexpr (std::formattable<T, char>) {
    //   fieldValStr = std::format("{}", field.value);
    // } else if constexpr (streamable<T>::value) {
    //   std::stringstream ss;
    //   ss << field.value;
    //   fieldValStr = std::format(", ({}:{})", field.name, ss.str());
    // } else if constexpr (toStringable<T>) {
    //   fieldValStr = field.value.toString();
    // }
    return fieldValStr;
  }
  std::mutex mtx_;
  std::map<std::string, std::vector<MetricDatapoint>> metrics;
};
} // namespace exchange::util::observability
#endif // EXCHANGE_UTIL_OBSERVABILITY_METRICRECORDER_H
