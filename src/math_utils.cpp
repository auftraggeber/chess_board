//
// Created by Jonas Langner on 25.07.23.
//

#include "../include/math_utils.h"
#include <iostream>

int math_utils::gcd(int i1, int i2) {
    i1 = std::abs(i1);
    i2 = std::abs(i2);

    int const max{std::max(i1,i2)};
    int const min{std::min(i1,i2)};

    if (min <= 0) {
        return max;
    }

    return gcd(min, max % min);
}