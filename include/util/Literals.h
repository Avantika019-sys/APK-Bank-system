

constexpr long double operator""_K(long double amount) { return amount * 1000; }
constexpr long double operator""_Million(long double amount) {
  return amount * 1000000;
}
