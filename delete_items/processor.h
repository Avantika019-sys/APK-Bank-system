#pragma once
#include "Tx.h"
#include <iostream>
#include <memory_resource>

// Transaction processor with strategy pattern
class TransactionProcessor {
private:
  std::pmr::vector<Tx> pending_transactions_;
  std::pmr::vector<Tx> completed_transactions_;

public:
  explicit TransactionProcessor(
      std::pmr::memory_resource *pool = std::pmr::get_default_resource())
      : pending_transactions_(pool), completed_transactions_(pool) {}

  template <typename T> void add_transaction(T &&transaction) {
    pending_transactions_.emplace_back(std::forward<T>(transaction));
  }

  // Process transactions with exception safety
  void process_pending() {
    for (auto &transaction : pending_transactions_) {
      try {
        bool success = transaction.process(
            [this](const auto &data) { return process_impl(data); });

        if (success) {
          transaction.mark_completed();
          completed_transactions_.push_back(std::move(transaction));
        }
      } catch (const exceptions::BankingException &e) {
        std::cerr << "Transaction failed: " << e.what() << "\n";
      }
    }

    // Remove processed transactions
    pending_transactions_.erase(
        std::remove_if(pending_transactions_.begin(),
                       pending_transactions_.end(),
                       [](const Transaction &t) { return t.is_completed(); }),
        pending_transactions_.end());
  }
};
