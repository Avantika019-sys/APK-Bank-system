
#ifndef BANK_LOGGER_H
#define BANK_LOGGER_H
#include <concepts>
#include <stdio.h>
#include <string>
template <typename T>
concept Stringable = requires(T obj) {
  { obj.toString() } -> std::same_as<std::string>;
};
enum class level {
  INFO,
  DEBUG,
  ERROR,
};
class logger {
public:
  logger(std::string id);
  void log(std::string msg, Stringable auto obj, level l);
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
