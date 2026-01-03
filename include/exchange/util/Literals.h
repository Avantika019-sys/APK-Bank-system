
#ifndef EXCHANGE_UTIL_LITERALS_H
#define EXCHANGE_UTIL_LITERALS_H
namespace exchange::util {
constexpr long double operator""_K(long double amount) { return amount * 1000; }
constexpr long double operator""_Mil(long double amount) {
  return amount * 1000000;
}
} // namespace exchange::util
#endif // EXCHANGE_UTIL_LITERALS_H
