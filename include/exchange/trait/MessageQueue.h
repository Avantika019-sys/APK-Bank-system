#include "exchange/asset.hpp"
#include "exchange/message/Info.h"
#include "exchange/message/MineEvent.h"
#include "exchange/message/Order.h"
#include "exchange/message/Stop.h"
#include <variant>

#ifndef EXCHANGE_TRAIT_MESSAGEQUEUE_H
#define EXCHANGE_TRAIT_MESSAGEQUEUE_H

using namespace exchange::message;
namespace exchange::trait {
template <typename T> class MessageQueue;

template <> struct MessageQueue<asset::Stock> {
  static int QueueCapacity() { return 100; }
};

template <> struct MessageQueue<asset::Crypto> {
  static int QueueCapacity() { return 80; }
};
} // namespace exchange::trait
#endif // EXCHANGE_TRAIT_MESSAGEQUEUE_H
