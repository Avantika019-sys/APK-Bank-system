//
// Created by ali on 11/13/25.
//

#include "stock/visitor.h"

namespace stock {
    void visitor::operator()(order &stockOrder) {
        stockOrder.prom.set_value(true);
    }

    void visitor::operator()(stateReq &stateReq) {
        state st;
        stateReq.prom.set_value(st);
    }

}
