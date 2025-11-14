//
// Created by ali on 11/13/25.
//
#include <future>
#include <set>
#include <boost/signals2/signal.hpp>

#include "visitor.h"
#include "messageQueue.h"
#include "../transaction.h"
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

namespace stock {
    class server {
    public:
        server();

        static server &getInstance();

        boost::signals2::signal<void (std::string stockName, double UpdatedPrice)> sig;

        void startUpdateStocksWorker();

        void startStockWorker();

        messageQueue msgQueue;

        void stopWorkers();

    private:
        std::vector<std::pair<std::string, double> > stocks;
        visitor orderVisitor;
        std::mutex mtx;
        bool run = true;
    };
}

#endif //BANK_STOCKSERVER_H
