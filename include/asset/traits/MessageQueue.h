
#include "asset/message/types/Info.h"
#include "asset/message/types/Order.h"
#include "asset/message/types/OrderEvent.h"
#include "asset/message/types/PortfolioTrend.h"
#include "asset/message/types/Stop.h"
#include "asset/message/types/crypto/MineEvent.h"

#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"
#include <variant>

#ifndef BANK_ASSETTRAITSMESSAGE_H
#define BANK_ASSETTRAITSMESSAGE_H

using namespace asset::message::types;
namespace asset::traits {
template <typename T> class MessageQueue {
public:
  using Variant = std::variant<OrderRequest<T>, InfoRequest<T>,
                               PortfolioTrendRequest<T>, OrderEvent<T>, Stop>;
  static int QueueCapacity() { return 100; }
};

template <> struct MessageQueue<types::Crypto> {
  using Variant =
      std::variant<OrderRequest<types::Crypto>, InfoRequest<types::Crypto>,
                   PortfolioTrendRequest<types::Crypto>,
                   OrderEvent<types::Crypto>, Stop, crypto::MineEvent>;
  static int QueueCapacity() { return 80; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSMESSAGE_H
