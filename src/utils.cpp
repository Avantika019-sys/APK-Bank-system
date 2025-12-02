#include "utils.h"

constexpr int operator"" _DKK (long double amount) {
    return static_cast<int>(amount * 100);
}
