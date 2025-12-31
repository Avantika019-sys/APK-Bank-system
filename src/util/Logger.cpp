#include "util/Logger.h"
#include <cstdio>
#include <ctime>
#include <format>
#include <stdexcept>
#include <string>
namespace util {
Logger::Logger(std::string id) {
  const std::string fileName = "logs/" + id + ".txt";
  fptrLogs_ = std::fopen(fileName.c_str(), "w");
  if (fptrLogs_ == NULL) {
    throw std::invalid_argument("Could not open log file");
  }
}

Logger::~Logger() {
  if (fptrLogs_ != nullptr) {
    std::fclose(fptrLogs_);
  }
}
Logger::Logger(Logger &&other) noexcept : fptrLogs_(other.fptrLogs_) {
  other.fptrLogs_ = nullptr;
}
Logger &Logger::operator=(Logger &&other) noexcept {
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
std::string Logger::levelToString(level l) {
  if (l == level::INFO) {
    return "INFO";
  }
  if (l == level::DEBUG) {
    return "DEBUG";
  }
  return "ERROR";
}
} // namespace util
