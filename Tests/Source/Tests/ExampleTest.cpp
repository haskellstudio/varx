/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 8:47:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "JuceHeader.h"

#include <iostream>

namespace juce
{
	std::ostream& operator<<(std::ostream& os, const Array<var> array) {
		os << "Hello World";
		return os;
	}
}

/*
namespace Catch {
	template<typename T>
	std::string toString(const juce::Array<T>& array) {
		std::string s = "{";
		s += "Hello";
		return s + "}";
	}
}
 */

#include "catch.hpp"

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

TEST_CASE("Observable::combineLatest with one other Observable") {
	auto s1 = Observable::just("Hello");
	auto s2 = Observable::just("World");
	
	auto combined = s1.combineLatest(s2, [](String s1, String s2) {
		return s1 + " " + s2 + "!";
	});
	
	String result;
	combined.subscribe([&](String s) {
		result = s;
	});
	
	REQUIRE(result == "Hello World!");
}

TEST_CASE("Observable::combineLatest with Array") {
	auto f = Observable::just(4.54);
	auto s = Observable::just(String(juce::CharPointer_UTF8("€")));
	auto comment = Observable::just("not a lot!");
	
	auto combined = f.combineLatest({s, comment}, [](juce::Array<var> values) {
		return String(float(values[0])) + " " + values[1].toString() + " is " + values[2].toString();
	});
	
	String result;
	combined.subscribe([&](String s) {
		result = s;
	});
	
	REQUIRE(result == "4.54 € is not a lot!");
}

namespace rxjuce {
	namespace util {
		String desc(const var& v) {return v.toString();}
		String desc(const String &s) { return s; }
		String desc(const juce::Array<var>& arr) { return "Some array description."; }
		String desc(const juce::StringArray &sa) {
			return ("{" + sa.joinIntoString(", ") + "}");
		}
		String desc(const juce::StringPairArray &spa) { return spa.getDescription(); }
		
	}
}


TEST_CASE("Observable::range") {
	struct TestSetup
	{
		var first, last;
		std::ptrdiff_t step;
		juce::Array<var> expected;
	};
	
	const juce::Array<TestSetup> setups({
		TestSetup({.first=0, .last=5, .step=1, .expected={0, 1, 2, 3, 4, 5}}),
		TestSetup({.first=17.5, .last=22.8, .step=2, .expected={17.5, 19.5, 21.5}})
	});
	
	juce::var b(true);
	juce::var strVar("Hello");
	juce::var dbl(3.14);
	juce::String s("Hey this is a string");
	juce::Array<var> arr({b, strVar, dbl, var(s)});
	juce::StringArray strArr({"Hello", "World!"});
	
	for (auto setup : setups) {
		auto range = Observable::range(setup.first, setup.last, setup.step);
		juce::Array<var> results;
		range.subscribe([&](var v) {
			results.add(v);
		});

		// REQUIRE(results == setup.expected);
	}
}
