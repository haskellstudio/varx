/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"
#include "rxjuce_LifetimeWatcherPoolFixture.h"

TEST_CASE("Observable::just",
		  "[Observable][Observable::just]")
{
	Array<var> results;
	
	IT("emits a single value on subscribe") {
		RxJUCECollectResults(Observable::just(18.3), results);
		
		RxJUCERequireResults(results, 18.3);
	}
	
	IT("notifies multiple subscriptions") {
		Observable o = Observable::just("Hello");
		RxJUCECollectResults(o, results);
		RxJUCECollectResults(o, results);
		
		RxJUCERequireResults(results, "Hello", "Hello");
	}
}

TEST_CASE("Observable::range",
		  "[Observable][Observable::range]")
{
	Array<var> results;
	
	IT("throws if first has an invalid type") {
		std::string error = "first has invalid type.";
		REQUIRE_THROWS_WITH(Observable::range("Hello", "Hello", 1), error);
		REQUIRE_THROWS_WITH(Observable::range(true, false, 1), error);
		REQUIRE_THROWS_WITH(Observable::range(Array<var>(), Array<var>(), 1), error);
		REQUIRE_THROWS_WITH(Observable::range(StringArray(), StringArray(), 1), error);
	}
	
	IT("throws if first and last have different types") {
		int _int = 3;
		int64 _int64 = 4;
		double _double = 5.66;
		
		std::string error = "first and last must have the same type.";
		REQUIRE_THROWS_WITH(Observable::range(_int, _int64, 1), error);
		REQUIRE_THROWS_WITH(Observable::range(_int, _double, 1), error);
		REQUIRE_THROWS_WITH(Observable::range(_int64, _double, 1), error);
	}
}


TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "Value and ValueSource lifetime",
				 "[Observable][Observable::fromValue]")
{
	// Create Value and subscribe
	auto value = std::make_shared<Value>("Initial");
	Array<var> results;
	RxJUCECollectResults(Observable::fromValue(*value), results);
	
	RxJUCERequireResults(results, "Initial");
	
	IT("still receives an item if the Value is destroyed immediately after calling setValue") {
		value->setValue("New Value");
		value.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial", "New Value");
	}
	
	IT("still receives an item if the Value is copied, the original is destroyed, and the copy sets a new value") {
		Value copy(*value);
		value.reset();
		RxJUCERunDispatchLoop();
		copy.setValue("New");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial", "New");
	}
}


TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "A Value notifies asynchronously",
				 "[Observable][Observable::fromValue]")
{
	Value value("Initial Value");
	Array<var> results;
	RxJUCECollectResults(Observable::fromValue(value), results);
	
	RxJUCERequireResults(results, "Initial Value");
	
	IT("emites only one item if the Value is set multiple times synchronously") {
		value = "2";
		value = "3";
		value = "4";
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial Value", "4");
	}
}


TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "A Value can have multiple Subscriptions or Observables",
				 "[Observable][Observable::fromValue]")
{
	// Create Value and subscribe
	Value value("Foo");
	Observable o = Observable::fromValue(value);
	Array<var> results;
	RxJUCECollectResults(o, results);
	
	RxJUCERequireResults(results, "Foo");
	
	IT("notifies multiple Subscriptions on subscribe") {
		Observable another = Observable::fromValue(value);
		RxJUCECollectResults(another, results);
	
		RxJUCERequireResults(results, "Foo", "Foo");
	}
	
	IT("notifies multiple Values referring to the same ValueSource") {
		Value anotherValue(value);
		Observable anotherObservable = Observable::fromValue(anotherValue);
		RxJUCECollectResults(anotherObservable, results);
		
		RxJUCERequireResults(results, "Foo", "Foo");
	}
	
	IT("notifies multiple Subscriptions if a Value is set multiple times") {
		RAIISubscription another = o.subscribe([&](String newValue) {
			results.add(newValue.toUpperCase());
		});
		
		value = "Bar";
		RxJUCERunDispatchLoop();
		
		value = "Baz";
		RxJUCERunDispatchLoop();
		
		REQUIRE(results.size() == 6);
		
		for (auto s : {"Foo", "FOO", "BAR", "Bar", "BAZ", "Baz"})
			REQUIRE(results.contains(s));
	}
}


TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "A Slider Value can be observed",
				 "[Observable][Observable::fromValue]")
{
	Slider slider;
	slider.setValue(7.6);
	Observable o = Observable::fromValue(slider.getValueObject());
	Array<var> results;
	RxJUCECollectResults(o, results);
	RxJUCERequireResults(results, 7.6);
	
	IT("emits once if the Slider is changed once") {
		slider.setValue(0.45);
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, 7.6, 0.45);
	}
	
	IT("emits just once if the Slider value changes rapidly") {
		for (double value : {3.41, 9.54, 4.67, 3.56})
			slider.setValue(value);
		
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, 7.6, 3.56);
	}
}
