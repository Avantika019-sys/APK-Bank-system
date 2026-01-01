#include "exchange/asset.hpp"
#include "exchange/message/Info.h"
#include "exchange/message/MineEvent.h"
#include "exchange/message/Order.h"
#include "exchange/message/PortfolioTrend.h"
#include "exchange/message/Stop.h"
#include <variant>

#ifndef EXCHANGE_TRAIT_MESSAGEQUEUE_H
#define EXCHANGE_TRAIT_MESSAGEQUEUE_H

using namespace exchange::message;
namespace exchange::trait {
template <typename T> class MessageQueue {
public:
  using Variant = std::variant<OrderRequest<T>, InfoRequest<T>,
                               PortfolioTrendRequest<T>, Stop>;
  static int QueueCapacity() { return 100; }
};

template <> struct MessageQueue<asset::Crypto> {
  using Variant =
      std::variant<OrderRequest<asset::Crypto>, InfoRequest<asset::Crypto>,
                   PortfolioTrendRequest<asset::Crypto>, Stop, MineEvent>;
  static int QueueCapacity() { return 80; }
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_MESSAGEQUEUE_H
