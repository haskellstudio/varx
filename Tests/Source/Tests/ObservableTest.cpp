/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"


TEST_CASE("Observable::just",
		  "[Observable][Observable::just]")
{
	Array<var> results;
	
	IT("emits a single value on subscribe") {
		RxJUCECollectItems(Observable::just(18.3), results);
		
		RxJUCERequireResults(results, 18.3);
	}
	
	IT("notifies multiple subscriptions") {
		Observable o = Observable::just("Hello");
		RxJUCECollectItems(o, results);
		RxJUCECollectItems(o, results);
		
		RxJUCERequireResults(results, "Hello", "Hello");
	}
}


TEST_CASE("Observable::range",
		  "[Observable][Observable::range]")
{
	Array<var> results;
	
	IT("emits integer numbers with an integer range") {
		RxJUCECollectItems(Observable::range(Range<int>(3, 7), 3), results);
		RxJUCERequireResults(results, 3, 6, 7);
	}
	
	IT("emits double numbers with a double range") {
		RxJUCECollectItems(Observable::range(Range<double>(17.5, 22.8), 2), results);
		RxJUCERequireResults(results, 17.5, 19.5, 21.5, 22.8);
	}
	
	IT("emits just start if start == end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 10), 1), results);
		RxJUCERequireResults(results, 10);
	}
	
	IT("emits just start if start > end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 9), 1), results);
		RxJUCERequireResults(results, 10);
	}
}


TEST_CASE("Observable::fromValue",
		  "[Observable][Observable::fromValue]")
{
	Value value("Initial Value");
	const auto observable = Observable::fromValue(value);
	Array<var> results;
	RxJUCECollectItems(observable, results);
	
	RxJUCERequireResults(results, "Initial Value");
	
	IT("emits if a copy of the Value sets a new value") {
		Value copy(value);
		copy.setValue("Set by copy");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial Value", "Set by copy");
	}
	
	IT("emites only one item if the Value is set multiple times synchronously") {
		value = "2";
		value = "3";
		value = "4";
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial Value", "4");
	}
	
	IT("notifies multiple Subscriptions on subscribe") {
		Observable another = Observable::fromValue(value);
		RxJUCECollectItems(another, results);
		
		RxJUCERequireResults(results, "Initial Value", "Initial Value");
	}
	
	IT("notifies multiple Values referring to the same ValueSource") {
		Value anotherValue(value);
		Observable anotherObservable = Observable::fromValue(anotherValue);
		RxJUCECollectItems(anotherObservable, results);
		
		RxJUCERequireResults(results, "Initial Value", "Initial Value");
	}
	
	IT("notifies multiple Subscriptions if a Value is set multiple times") {
		ScopedSubscription another = observable.subscribe([&](String newValue) {
			results.add(newValue.toUpperCase());
		});
		
		value = "Bar";
		RxJUCERunDispatchLoop();
		
		value = "Baz";
		RxJUCERunDispatchLoop();
		
		REQUIRE(results.size() == 6);
		
		// Subscribers are notified in no particular order
		for (auto s : {"Initial Value", "INITIAL VALUE", "BAR", "Bar", "BAZ", "Baz"})
			REQUIRE(results.contains(s));
	}
}


TEST_CASE("Observable::fromValue lifetime",
		  "[Observable][Observable::fromValue]")
{
	// Create an Observable from a Value
	Value value("Initial");
	auto source = std::make_shared<Observable>(Observable::fromValue(value));
	
	// Create another Observable from the source Observable
	auto mapped = source->map([](String s){ return s; });
	
	// Collect items from the mapped Observable
	Array<var> results;
	RxJUCECollectItems(mapped, results);
	
	RxJUCERequireResults(results, "Initial");
	
	IT("emits items when the source Observable is alive") {
		value.setValue("New Value");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial", "New Value");
	}
	
	IT("stops emitting items as soon as the source Observable is destroyed") {
		source.reset();
		value.setValue("Two");
		value.setValue("Three");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial");
	}
	
	IT("does not emit an item if the Observable is destroyed immediately after calling setValue") {
		value.setValue("New Value");
		source.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial");
	}
	
	IT("continues to emit items if the source Observable is copied and then destroyed") {
		auto copy = std::make_shared<Observable>(*source);
		Array<var> copyResults;
		RxJUCECollectItems(*copy, copyResults);
		
		RxJUCERequireResults(copyResults, "Initial");
		
		source.reset();
		RxJUCERunDispatchLoop();
		value.setValue("New");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(copyResults, "Initial", "New");
	}
}


TEST_CASE("Observable::fromValue with a Slider",
		  "[Observable][Observable::fromValue]")
{
	Slider slider;
	slider.setValue(7.6);
	Observable o = Observable::fromValue(slider.getValueObject());
	Array<var> results;
	RxJUCECollectItems(o, results);
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
