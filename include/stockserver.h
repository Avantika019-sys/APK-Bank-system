//
// Created by ali on 11/13/25.
//
#include <set>
#include <boost/signals2/signal.hpp>
#include "messageQueue.h"
#include "order.h"
#include "transaction.h"
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H


typedef std::variant<buyOrder,sellOrder> orderVariant;


struct orderVisitor {
    void operator()(const buyOrder& bo);
    void operator()(const sellOrder& so);
};


class stockserver {
    public:
    stockserver();
    boost::signals2::signal<void (std::string stockName, double UpdatedPrice)>sig;
    void startUpdateWorker();
    void startOrderWorker();
private:
    std::vector<std::pair<std::string,double>> stocks;
    messageQueue<orderVariant> msgQueue;
    orderVisitor orderVisitor;
};


#endif //BANK_STOCKSERVER_H