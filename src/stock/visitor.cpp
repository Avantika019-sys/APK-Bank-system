#include "stock/visitor.h"

namespace stock {
order::order() {}

info::info(std::string name) : stockName(name) {}

void visitor::operator()(order &stockOrder) { stockOrder.prom.set_value(true); }

void visitor::operator()(info &stateReq) { stateReq.prom.set_value(10); }
} // namespace stock
