
#include <functional>
#include <string>
#ifndef BANK_ORDEREVENT_H
#define BANK_ORDEREVENT_H
namespace asset::messages {
struct OrderEvent {
  std::function<void(std::string assetName, int qty)> cb;
};
} // namespace asset::messages
#endif // BANK_ORDEREVENT_H
