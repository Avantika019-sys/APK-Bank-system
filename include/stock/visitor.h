//
// Created by ali on 11/13/25.
//

#ifndef BANK_VISITOR_H
#define BANK_VISITOR_H
#include <future>

#include "transaction.h"

namespace stock {


    struct order {
        order();
        stockTransaction tx;
        std::promise<bool> prom;
    };

    struct state {
        int quantityOwned;
        double valuePerStock;
    };
    struct stateReq {
        stateReq();
        std::string name;
        std::promise<state> prom;
    };

    typedef std::variant<order,stateReq> stockVariant;

    struct visitor {
        void operator()( order& bo);
        void operator()( stateReq& so);
    };

}
#endif //BANK_VISITOR_H