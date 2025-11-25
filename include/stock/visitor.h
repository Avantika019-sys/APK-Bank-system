//
// Created by ali on 11/13/25.
//

#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include <future>

#include "transaction.h"

namespace stock {
  struct order {
    order(stockTx tx);

    stockTx tx;
    std::promise<bool> prom; // result of the order
  };

  struct info {
    explicit info(std::string name);

    std::string stockName;
    std::promise<int> prom; // price per stock
  };

  typedef std::variant<order, info> variant;

  struct visitor {
    void operator()(order &bo);

    void operator()(info &so);
  };
} // namespace stock
#endif // BANK_VISITOR_H
