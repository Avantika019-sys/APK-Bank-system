
#include <functional>
#include <string>
namespace bank::server::messages {
struct OrderEvent {
  std::function<void(std::string assetName, int qty)> cb;
};
} // namespace bank::server::messages
