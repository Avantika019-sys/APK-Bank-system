#include "currencyExchange.hpp"

currencyExchange::currencyExchange(std::string from, std::string to, double rate)
    : fromCurrency(from), toCurrency(to), exchangeRate(rate) {
    log.addEntry("Currency exchange created from " + from + " to " + to + " with rate " + std::to_string(rate));
}

void currencyExchange::setExchangeRate(double rate) {
    exchangeRate = rate;
    log.addEntry("Exchange rate updated to " + std::to_string(rate));
}

double currencyExchange::convert(double amount) {
    double convertedAmount = amount * exchangeRate;
    log.addEntry("Converted " + std::to_string(amount) + " " + fromCurrency + " to " + std::to_string(convertedAmount) + " " + toCurrency);
    return convertedAmount;
}

void currencyExchange::showLog() {
    log.showEntries();
}

std::string currencyExchange::getFromCurrency() const {
    return fromCurrency;
}

std::string currencyExchange::getToCurrency() const {
    return toCurrency;
}

double currencyExchange::getExchangeRate() const {
    return exchangeRate;
}

