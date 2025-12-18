#include <string>
#include <unordered_map>
#include "log.hpp"


template<typename T>
concept CurrencyCode = requires(T s) {
    { std::string{s} };
};

class currencyExchange {
private:
    std::string fromCurrency;
    std::string toCurrency;
    std::unordered_map<std::string, int> rates;
    int exchangeRate;
    Log log;

public:
    currencyExchange(CurrencyCode auto from, CurrencyCode auto to, int rate);

    currencyExchange(const currencyExchange&) = delete;
    currencyExchange& operator=(const currencyExchange&) = delete;
    ~currencyExchange() = default;

    void setExchangeRate(int rate);
    int convert(int amount);
};
