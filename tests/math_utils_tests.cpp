//
// Created by Jonas Langner on 25.07.23.
//

#include <catch2/catch_all.hpp>
#include "../include/math_utils.h"
#include "../src/math_utils.cpp"

int main( int argc, char* argv[] ) {
    // your setup ...

    int result = Catch::Session().run( argc, argv );

    // your clean-up...

    return result;
}

TEST_CASE("gcd") {
    using namespace math_utils;

    auto const test = [](int const i1, int const i2, int const r) {
        REQUIRE(gcd(i1,i2) == r);
    };

    test(3,4,1);
    test(400,75,25);
    test(75,400,25);
    test(0,0,0);
    test(-4, -6, 2);
}