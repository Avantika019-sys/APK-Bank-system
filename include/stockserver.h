//
// Created by ali on 11/13/25.
//
#include <set>
#include <boost/signals2/signal.hpp>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H

class stockserver {
    public:
    stockserver();
    boost::signals2::signal<void (std::string stockName, double UpdatedPrice)>sig;
    void startWorker();
private:
    std::vector<std::pair<std::string,double>> stocks;
};



#endif //BANK_STOCKSERVER_H