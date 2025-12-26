
#ifndef BANK_LOGGER_H
#define BANK_LOGGER_H
#include <chrono>
#include <ctime>
#include <format>
#include <sstream>
#include <stdio.h>
#include <string>
#include <type_traits>
namespace util {

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
template <typename T> struct field {
  field(std::string name, T value) : name(name), value(value) {}
  std::string name;
  T value;
};
class Logger {
public:
  Logger(std::string id);
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
  std::enable_if<streamable<T>::value>::type log(std::string msg, level l,
                                                 field<T> field, Args... args) {
    std::stringstream ss;
    ss << field.value;
    msg += std::format(", ({}:{})", field.name, ss.str());
    log(msg, l, args...);
  }
  template <typename T, typename... Args>
  void log(std::string msg, level l, field<T> field, Args... args)
    requires(!streamable<T>::value && std::formattable<T, char>)
  {
    msg += std::format(", ({}:{})", field.name, field.value);
    log(msg, l, args...);
  }
  ~Logger();
  Logger(Logger &&other) noexcept;
  Logger &operator=(Logger &&other) noexcept;

private:
  Logger(const Logger &other);
  Logger &operator=(const Logger &other);
  std::string levelToString(level l);
  FILE *fptrLogs_;
};
} // namespace util
#endif // BANK_LOGGER_H
