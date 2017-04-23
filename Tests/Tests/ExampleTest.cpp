/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 8:47:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "catch.hpp"

unsigned int Factorial( unsigned int number ) {
	return number > 1 ? Factorial(number-1)*number : 1;
}

TEST_CASE("Factorials are computed") {
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628801);
}
