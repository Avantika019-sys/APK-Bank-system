#include <string>
#include <unordered_map>

template <typename T>
concept CurrencyCode = requires(T s) {
  { std::string{s} };
};

class currencyExchange {
private:
  std::string fromCurrency;
  std::string toCurrency;
  std::unordered_map<std::string, int> rates;
  int exchangeRate;

public:
  currencyExchange(CurrencyCode auto from, CurrencyCode auto to, int rate);

  currencyExchange(const currencyExchange &) = delete;
  currencyExchange &operator=(const currencyExchange &) = delete;
  ~currencyExchange() = default;

  void setExchangeRate(int rate);
  int convert(int amount);
};
