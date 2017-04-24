/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 8:47:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "catch.hpp"

#define DONT_SET_USING_JUCE_NAMESPACE 1
#include "JuceHeader.h"

using namespace rxjuce;
using juce::var;
using juce::Value;

TEST_CASE("Observable::just") {
	double result = 0;
	
	Observable::just(3.14).subscribe([&](var newValue) {
		result = newValue;
	});
	
	REQUIRE(result == 3.14);
}

TEST_CASE("Value Observable") {
	Value value(17);
	
	double result = 0;
	
	auto subscription = Observable(value).subscribe([&](var newValue) {
		result = newValue;
	});
	
	REQUIRE(result == 17);
	
	value = 42;

	juce::MessageManager::getInstance()->runDispatchLoopUntil(0);
	
	REQUIRE(result == 42);
}
