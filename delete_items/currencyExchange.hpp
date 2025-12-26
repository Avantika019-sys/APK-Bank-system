#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <concepts>

class currencyExchange{
  public:
  using RateFunc = std::function<double(double)>;
  //void addRate(const std::string& from, const std::string& to, RateFunc func);
  void addRate(const std::string& to, RateFunc func);
  template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>> 
  //double convert (const std::string& from, const std::string& to, RateFunc func); 
  double convert (const std::string& to, T amount) const{
    auto it = rate.find(to);
    if(it ==  rates.end())
      throw std::runtime_error("This currency is not supported")
    return it ->second (static_cast<double(amount));
  }
  
  const std::string base() const{return baseCurrency;}

  private:
  const std::string baseCurrency = "DKK";
  std::unordered_map<std::string, RateFunc> rates;
}



























// #include <string>
// #include <unordered_map>

// template <typename T>
// concept CurrencyCode = requires(T s) {
//   { std::string{s} };
// };

// class currencyExchange {
// private:
//   std::string fromCurrency;
//   std::string toCurrency;
//   std::unordered_map<std::string, int> rates;
//   int exchangeRate;

// public:
//   currencyExchange(CurrencyCode auto from, CurrencyCode auto to, int rate);
//   currencyExchange(const currencyExchange &) = delete;
//   currencyExchange &operator=(const currencyExchange &) = delete;
//   ~currencyExchange() = default;

//   void setExchangeRate(int rate);
//   int convert(int amount);
// };
