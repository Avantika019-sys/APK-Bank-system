#include "logger.h"
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

void logger::log(std::string msg, Stringable auto arg, level l) {
  std::string levelStr = levelToString(l);
  auto timeNow = std::time(nullptr);
  auto timeNowStr = std::string(std::ctime(&timeNow));
  std::string logMsg = std::format("[{}][{}] msg: {} obj: {}", levelStr,
                                   timeNowStr, msg, arg.toString());
  const char *buffer = logMsg.c_str();
  fputs(buffer, fptrLogs_);
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
  if (l == level::ERROR) {
    return "ERROR";
  }
  return "";
}
