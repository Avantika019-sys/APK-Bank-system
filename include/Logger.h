
#ifndef BANK_LOGGER_H
#define BANK_LOGGER_H
#include <chrono>
#include <ctime>
#include <format>
#include <sstream>
#include <stdio.h>
#include <string>
#include <type_traits>
template <typename T, typename = void> struct streamable : std::false_type {};
template <typename T>
struct streamable<T, std::void_t<decltype(std::declval<std::ostream &>()
                                          << std::declval<T>())>>
    : std::true_type {};

enum class level {
  INFO,
  DEBUG,
  ERROR,
};
class logger {
public:
  logger(std::string id);
  template <typename T>
  std::enable_if<streamable<T>::value>::type log(std::string msg, level l,
                                                 std::string key, T val) {
    std::stringstream ss;
    ss << val;
    std::string levelStr = levelToString(l);
    auto now = std::chrono::system_clock::now();
    std::string logMsg =
        std::format("[{}][{:%Y-%m-%d %H:%M:%S}] msg: {} key: {} val: {}",
                    levelStr, now, msg, key, ss.str());
    const char *buffer = logMsg.c_str();
    fputs(buffer, fptrLogs_);
    fflush(fptrLogs_);
  }

  template <typename T>
  void log(std::string msg, level l, std::string key, T val)
    requires(!streamable<T>::value && std::formattable<T, char>)
  {

    std::string levelStr = levelToString(l);
    auto timeNow = std::time(nullptr);
    auto timeNowStr = std::string(std::ctime(&timeNow));
    std::string logMsg = std::format("[{}][{}] msg: {} key: {} val: {}",
                                     levelStr, timeNowStr, msg, key, val);
    const char *buffer = logMsg.c_str();
    fputs(buffer, fptrLogs_);
    fflush(fptrLogs_);
  }
  void log(std::string msg, level l) {

    std::string levelStr = levelToString(l);
    auto timeNow = std::time(nullptr);
    auto timeNowStr = std::string(std::ctime(&timeNow));
    std::string logMsg =
        std::format("[{}][{}] msg: {}", levelStr, timeNowStr, msg);
    const char *buffer = logMsg.c_str();
    fputs(buffer, fptrLogs_);
    fflush(fptrLogs_);
  }
  ~logger();
  logger(logger &&other) noexcept;
  logger &operator=(logger &&other) noexcept;

private:
  logger(const logger &other);
  logger &operator=(const logger &other);
  std::string levelToString(level l);
  FILE *fptrLogs_;
};

#endif // BANK_LOGGER_H
// std::pmr::vector<string> entries;
// std::pmr::memory_resource* memRes = std::pmr::get_default_resource();
