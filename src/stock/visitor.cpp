#include "stock/visitor.h"

namespace stock {

info::info(std::string name) : stockName(name) {}
order::order(string stockName, int amountOfStocks, orderType type)
    : stockName(stockName), amountOfStocks(amountOfStocks), type(type) {}
void visitor::operator()(order &stockOrder) { stockOrder.prom.set_value(true); }

void visitor::operator()(info &stateReq) { stateReq.prom.set_value(10); }
} // namespace stock
