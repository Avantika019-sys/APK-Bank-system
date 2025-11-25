
#include "transaction.h"
#include <chrono>
#include <exception>
#include <optional>
#include <string>

using Timestamp = std::chrono::system_clock::time_point;

enum class ErrorCode {
  INSUFFICIENT_FUNDS,
  SYSTEM_FAILURE,
  // We can keep adding
};

// Custom exception hierarchy using inheritance
class BankingException : public std::exception {
  std::string message_;
  ErrorCode code_;
  Timestamp timestamp_;

public:
  BankingException(std::string msg, ErrorCode code)
      : message_(std::move(msg)), code_(code),
        timestamp_(std::chrono::system_clock::now()) {}

  virtual const char *what() const noexcept override {
    return message_.c_str();
  }
  virtual ErrorCode get_code() const noexcept { return code_; }
};

class TransactionException : public BankingException {
  Transaction *transaction_id_;

public:
  TransactionException(std::string msg, ErrorCode code, Transaction *tid)
      : BankingException(std::move(msg), code), transaction_id_(tid) {}
};

class InsufficientFundsException : public TransactionException {
  float available_;
  float requested_;

public:
  InsufficientFundsException(Transaction *tid, float avail, float req)
      : TransactionException("Insufficient funds",
                             ErrorCode::INSUFFICIENT_FUNDS, tid),
        available_(avail), requested_(req) {}
};

// Exception-safe banking operations
template <typename Operation>
auto execute_banking_operation(Operation &&op)
    -> std::optional<decltype(op())> {

  try {
    return std::forward<Operation>(op)();
  } catch (const InsufficientFundsException &e) {
    logger::warn("Insufficient funds: {}", e.what());
    return std::nullopt;
  } catch (const TransactionException &e) {
    logger::error("Transaction failed: {}", e.what());
    return std::nullopt;
  } catch (const std::exception &e) {
    logger::critical("System error: {}", e.what());
    return std::nullopt;
  }
}
