
#include "asset/traits/Precision.h"
#include "asset/traits/Server.h"
#ifndef BANK_ASSETMESSAGESCRYPTOMINEEVENT_H
#define BANK_ASSETMESSAGESCRYPTOMINEEVENT_H
namespace asset::message::types::crypto {
struct MineEvent {
  std::string cryptoName;
  double qty;
};
} // namespace asset::message::types::crypto
#endif // BANK_ASSETMESSAGESCRYPTOMINEEVENT_H
