
#ifndef EXCHANGE_UTIL_OBSERVABILITY_FIELD_H
#define EXCHANGE_UTIL_OBSERVABILITY_FIELD_H
#include <string>
namespace exchange::util::observability {
template <typename T> struct field {
  std::string name;
  T value;
};
} // namespace exchange::util::observability
#endif // EXCHANGE_UTIL_OBSERVABILITY_FIELD_H
