#include <string>
#include <unordered_map>
#include "log.hpp"

class currencyExchange {
private:
    std::string fromCurrency;
    std::string toCurrency;
    std::unordered_map<std::string, double> rates;
    double exchangeRate;
    Log log;

public:
    currencyExchange(std::string from, std::string to, double rate);
    void setExchangeRate(double rate);
    void showLog();
    double convert(double amount);
    std::string getFromCurrency() const;
    std::string getToCurrency() const;
    double getExchangeRate() const;
};