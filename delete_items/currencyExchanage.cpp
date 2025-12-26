#include "currencyExchange.hpp"
#include <utility>

void currencyExchange::addRate(const std::string& to, RateFunc func){
    rates[to] = std::move(func);

}


































// #include "currencyExchange.hpp"
// #include <stdexcept>
// #include <iostream>

// currencyExchange::currencyExchange(CurrencyCode auto from,
//                                    CurrencyCode auto to,
//                                    int rate)
//     : fromCurrency(std::string{from}),
//       toCurrency(std::string{to}),
//       exchangeRate(rate)
// {
//     if (rate <= 0) {
//         throw std::invalid_argument("Exchange rate must be positive");
//     }

    
//     rates[toCurrency] = rate;

//     // could maybe have log here 
// }

// void currencyExchange::setExchangeRate(int rate) {
//     if (rate <= 0) {
//         throw std::invalid_argument("Exchange rate must be positive");
//     }

//     exchangeRate = rate;
//     rates[toCurrency] = rate;

//     // could maybe have log here 
// }


// int currencyExchange::convert(int amount) {
//     if (amount < 0) {
//         throw std::invalid_argument("Amount cannot be negative");
//     }

//     int result = amount * exchangeRate;

//     // could maybe have log here 

//     return result;
// }

