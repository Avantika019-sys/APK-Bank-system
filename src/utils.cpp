//
// Created by ali on 11/13/25.
//

#include "utils.h"

constexpr int operator"" _DKK (long double amount) {
    return static_cast<int>(amount * 100);
}