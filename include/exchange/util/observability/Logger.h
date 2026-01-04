
#ifndef EXCHANGE_UTIL_OBSERVABILITY_LOGGER_H
#define EXCHANGE_UTIL_OBSERVABILITY_LOGGER_H
#include "Field.h"
#include <chrono>
#include <concepts>
#include <format>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <type_traits>
namespace exchange::util::observability {

template <typename T>
concept toStringable = requires(T t) {
  { t.toString() } -> std::same_as<std::string>;
};
template <typename T>
concept isLoggable = (std::formattable<T, char> || toStringable<T>);
enum class level {
  INFO,
  DEBUG,
  ERROR,
};
class Logger {
public:
  Logger() = default;
  Logger(std::string fileName) {
    fptrLogs_ = std::fopen(fileName.c_str(), "w");
    if (fptrLogs_ == NULL) {
      throw std::runtime_error("Could not open log file");
    }
  }
  void log(std::string msg, level l) {
    std::string levelStr = levelToString(l);
    auto now = std::chrono::system_clock::now();
    std::string finalMsg =
        std::format("[{}][{:%Y-%m-%d %H:%M:%S}] {}\n", levelStr, now, msg);
    const char *buffer = finalMsg.c_str();
    fputs(buffer, fptrLogs_);
    fflush(fptrLogs_);
  }
  template <typename T, typename... Args>
  void log(std::string msg, level l, field<T> field, Args... args)
    requires isLoggable<T>
  {
    if constexpr (std::formattable<T, char>) {
      msg += std::format(", ({}:{})", field.name, field.value);
    } else {
      msg += std::format(", ({}:{})", field.name, field.value.toString());
    }
    log(msg, l, args...);
  }
  ~Logger() {
    if (fptrLogs_ != nullptr) {
      std::fclose(fptrLogs_);
    }
  }
  Logger(Logger &&other) noexcept : fptrLogs_(other.fptrLogs_) {
    other.fptrLogs_ = nullptr;
  }
  Logger &operator=(Logger &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (fptrLogs_ != nullptr) {
      fclose(fptrLogs_);
    }
    fptrLogs_ = other.fptrLogs_;
    other.fptrLogs_ = nullptr;
    return *this;
  }

private:
  std::string levelToString(level l) {
    if (l == level::INFO) {
      return "INFO";
    }
    if (l == level::DEBUG) {
      return "DEBUG";
    }
    return "ERROR";
  }
  FILE *fptrLogs_;
};
} // namespace exchange::util::observability
#endif // EXCHANGE_UTIL_OBSERVABILITY_LOGGER_H
