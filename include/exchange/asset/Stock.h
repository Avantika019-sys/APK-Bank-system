
#include "exchange/currency/DKK.h"
#include "exchange/util/observability/MonitorResource.h"
#include <boost/signals2/signal.hpp>
#include <string>
#ifndef EXCHANGE_ASSET_STOCK_H
#define EXCHANGE_ASSET_STOCK_H
namespace exchange::asset {
typedef boost::signals2::signal<void(currency::DKK UpdatedPrice)> UpdateSignal;
class Stock {
public:
  Stock(std::string name, util::observability::MonitorResource *s);

  Stock(const Stock &other) = delete;
  Stock &operator=(const Stock &other) = delete;

  Stock(Stock &&other) noexcept;
  Stock &operator=(Stock &&other) noexcept;
  ~Stock();

  std::string name_;
  std::string symbol;
  std::pmr::vector<currency::DKK> unitPriceOverTime_;
  UpdateSignal *sig_;
};
} // namespace exchange::asset

#endif // EXCHANGE_ASSET_STOCK_H
