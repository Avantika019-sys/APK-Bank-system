
#include "asset/messages/Info.h"
#include "asset/messages/Order.h"
#include "asset/messages/OrderEvent.h"
#include "asset/messages/PortfolioTrend.h"
#include "asset/messages/Stop.h"
#include "asset/messages/crypto/MineEvent.h"

#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"
#include <variant>

#ifndef BANK_ASSETTRAITSMESSAGE_H
#define BANK_ASSETTRAITSMESSAGE_H

namespace asset::traits {
template <typename T> class MessageQueue {
public:
  using Variant =
      std::variant<messages::OrderRequest<T>, messages::InfoRequest<T>,
                   messages::PortfolioTrendRequest<T>, messages::OrderEvent<T>,
                   messages::Stop>;
  static int QueueCapacity() { return 100; }
};

template <> struct MessageQueue<types::Crypto> {
  using Variant = std::variant<messages::OrderRequest<types::Crypto>,
                               messages::InfoRequest<types::Crypto>,
                               messages::PortfolioTrendRequest<types::Crypto>,
                               messages::OrderEvent<types::Crypto>,
                               messages::Stop, messages::crypto::MineEvent>;
  static int QueueCapacity() { return 80; }
};
} // namespace asset::traits
#endif // BANK_ASSETTRAITSMESSAGE_H
