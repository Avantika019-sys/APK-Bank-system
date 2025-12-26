
#include <functional>
#include <string>
namespace asset::messages {
struct OrderEvent {
  std::function<void(std::string assetName, int qty)> cb;
};
} // namespace asset::messages
