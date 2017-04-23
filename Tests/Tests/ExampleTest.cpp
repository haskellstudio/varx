/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 8:47:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "catch.hpp"

#include "JuceHeader.h"

using namespace rxjuce;

TEST_CASE("Observable::just") {
	double value = 0;
	
	Observable::just(3.14).subscribe([&](var newValue) {
		value = newValue;
	});
	
	REQUIRE(value == 3.14);
}
