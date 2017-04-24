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
using juce::String;

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
	
	auto subscription = Observable(value).subscribe([&](double newValue) {
		result = newValue;
	});
	
	REQUIRE(result == 17);
	
	value = 42;

	juce::MessageManager::getInstance()->runDispatchLoopUntil(0);
	
	REQUIRE(result == 42);
}

TEST_CASE("Observable::map") {
	bool b = 3.14;
	auto stringObservable = Observable::just("17.25");
	
	auto floatObservable = stringObservable.map([](String s) {
		return s.getFloatValue() * 2;
	});
	
	auto secondStringObservable = floatObservable.map([](float f) {
		return String(f) + " years.";
	});
	
	
	String result;
	secondStringObservable.subscribe([&](String s) {
		result = s;
	});
	
	REQUIRE(result == "34.5 years.");
}

TEST_CASE("Observable::combineLatest") {
	auto f = Observable::just(4.54);
	auto s = Observable::just(String(juce::CharPointer_UTF8("€")));
	auto comment = Observable::just("not a lot!");
	
	juce::Array<Observable> others;
	others.add(s);
	others.add(comment);
	auto combined = f.combineLatest(others, [](juce::Array<var> values) {
		return String(float(values[0])) + " " + values[1].toString() + " is " + values[2].toString();
	});
	
	String result;
	combined.subscribe([&](String s) {
		result = s;
	});
	
	REQUIRE(result == "4.54 € is not a lot!");
}
