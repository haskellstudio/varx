/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 8:47:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

TEST_CASE("Observable::just") {
	RxJUCECollectResult(Observable::just(3.14), result);
	
	REQUIRE(result == var(3.14));
}

TEST_CASE("Observable::fromValue") {
	Value value(17);
	
	RxJUCECollectResult(Observable::fromValue(value), result);
	
	REQUIRE(result == var(17));
	
	value = 42;

	MessageManager::getInstance()->runDispatchLoopUntil(0);
	
	REQUIRE(result == var(42));
}

TEST_CASE("Observable::map") {
	auto stringObservable = Observable::just("17.25");
	
	auto floatObservable = stringObservable.map([](String s) {
		return s.getFloatValue() * 2;
	});
	
	auto secondStringObservable = floatObservable.map([](float f) {
		return String(f) + " years.";
	});
	
	RxJUCECollectResult(secondStringObservable, result);
	
	REQUIRE(result == "34.5 years.");
}

TEST_CASE("Observable::combineLatest with one other Observable") {
	auto s1 = Observable::just("Hello");
	auto s2 = Observable::just("World");
	
	auto combined = s1.combineLatest(s2, [](String s1, String s2) {
		return s1 + " " + s2 + "!";
	});
	
	RxJUCECollectResult(combined, result);
	
	REQUIRE(result == "Hello World!");
}

TEST_CASE("Observable::range") {
	struct TestSetup
	{
		var first, last;
		std::ptrdiff_t step;
		Array<var> expected;
	};
	
	const Array<TestSetup> setups({
		TestSetup({.first=0, .last=5, .step=1, .expected={0, 1, 2, 3, 4, 5}}),
		TestSetup({.first=17.5, .last=22.8, .step=2, .expected={17.5, 19.5, 21.5, 22.8}})
	});
	
	var b(true);
	var strVar("Hello");
	var dbl(3.14);
	String s("Hey this is a string");
	Array<var> arr({b, strVar, dbl, var(s)});
	StringArray strArr({"Hello", "World!"});
	
	for (auto setup : setups) {
		RxJUCECollectResults(Observable::range(setup.first, setup.last, setup.step), results);

		REQUIRE(results == setup.expected);
	}
}

TEST_CASE("Observable::create") {
	auto o = Observable::create([](Subscriber s) {
		s.onNext(3);
		s.onNext("Hello there!");
		s.onNext(14.33);
	});
	
	RxJUCECollectResults(o, results);
	
	REQUIRE(results == Array<var>({var(3), var("Hello there!"), var(14.33)}));
}

TEST_CASE("Observable::create with async onSubscribe") {
	auto o = Observable::create([](Subscriber s) {
		MessageManager::callAsync([s] () {
			s.onNext(3.14);
			s.onNext("Test");
		});
	});
	
	RxJUCECollectResults(o, results);
	
	MessageManager::getInstance()->runDispatchLoopUntil(0);
	
	REQUIRE(results == Array<var>({var(3.14), var("Test")}));
}
