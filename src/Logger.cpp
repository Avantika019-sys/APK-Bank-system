#include "Logger.h"
#include <cstdio>
#include <ctime>
#include <format>
#include <stdexcept>
#include <string>
logger::logger(std::string id) {
  const std::string fileName = id + ".txt";
  fptrLogs_ = std::fopen(fileName.c_str(), "w");
  if (fptrLogs_ == NULL) {
    throw std::invalid_argument("Could not open log file");
  }
}

logger::~logger() {
  if (fptrLogs_ != nullptr) {
    std::fclose(fptrLogs_);
  }
}
logger::logger(logger &&other) noexcept : fptrLogs_(other.fptrLogs_) {
  other.fptrLogs_ = nullptr;
}
logger &logger::operator=(logger &&other) noexcept {
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
std::string logger::levelToString(level l) {
  if (l == level::INFO) {
    return "INFO";
  }
  if (l == level::DEBUG) {
    return "DEBUG";
  }
  return "ERROR";
}
