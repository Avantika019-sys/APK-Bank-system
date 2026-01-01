
#ifndef EXCHANGE_MESSAGE_MINEEVENT_H
#define EXCHANGE_MESSAGE_MINEEVENT_H
#include <string>
namespace exchange::message {
struct MineEvent {
  std::string cryptoName;
  double qty;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_MINEEVENT_H
