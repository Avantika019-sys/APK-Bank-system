//
// Created by ali on 11/13/25.
//

#ifndef BANK_ORDER_H
#define BANK_ORDER_H
#include <string>
#include <variant>


struct buyOrder {
    std::string stockName;
    double amount;
};

struct sellOrder {
    std::string stockName;
    double amount;
};


#endif //BANK_ORDER_H