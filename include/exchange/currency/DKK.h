

#ifndef EXCHANGE_CURRENCY_DKK_H
#define EXCHANGE_CURRENCY_DKK_H
#include <format>
#include <iomanip>
#include <stdexcept>
#include <string>
namespace exchange::currency {
struct DKK {
public:
  DKK(double amount) : amount_(amount) {}
  double value() const { return amount_; }
  std::string toString() const { return std::format("{:.2f} DKK", amount_); }
  friend std::ostream &operator<<(std::ostream &os, const DKK &dkk) {
    os << std::fixed << std::setprecision(2) << dkk.amount_ << " DKK";
    return os;
  }
  DKK &operator+=(const DKK &other) {
    amount_ += other.amount_;
    return *this;
  }
  DKK &operator-=(const DKK &other) {
    amount_ -= other.amount_;
    return *this;
  }

private:
  double amount_;
};

inline DKK operator+(DKK l, DKK r) { return DKK(l.value() + r.value()); }
inline DKK operator-(DKK l, DKK r) { return DKK(l.value() - r.value()); }
inline DKK operator/(DKK l, DKK r) { return DKK(l.value() / r.value()); }
inline DKK operator*(DKK l, DKK r) { return DKK(l.value() * r.value()); }

} // namespace exchange::currency
#endif // EXCHANGE_CURRENCY_DKK_H
