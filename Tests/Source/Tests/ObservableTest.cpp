/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

SCENARIO("Value and ValueSource lifetime",
		 "[Observable][Observable::fromValue]")
{
	GIVEN("an Observable from a Value") {
		auto value = std::make_shared<Value>("Initial");
		Array<var> results;
		RxJUCECollectResults(Observable::fromValue(*value), results);
		RxJUCERequireResults(results, "Initial");
		
		WHEN("the Value is set to a new value and destroyed immediately") {
			value->setValue("New Value");
			value.reset();
			RxJUCERunDispatchLoop();
			
			THEN("the Observable still emits an item") {
				RxJUCERequireResults(results, "Initial", "New Value");
			}
		}
		
		WHEN("a copy of the Value is made, the original is destroyed, and the copy sets a new value") {
			Value copy(*value);
			value.reset();
			RxJUCERunDispatchLoop();
			copy.setValue("New");
			RxJUCERunDispatchLoop();
			
			THEN("the subscription still receives items") {
				RxJUCERequireResults(results, "Initial", "New");
			}
		}
		
		WHEN("the value is alive") {
			THEN("there are two references to its source (one from itself, and one from the LifetimeWatcher") {
				REQUIRE(value->getValueSource().getReferenceCount() == 2);
			}
		}
	}
}

TEST_CASE("A Value can have multiple Subscriptions or Observables",
		  "[Observable][Observable::fromValue]")
{
	GIVEN("a Subscription to a Value Observable") {
		Value value("Foo");
		Observable o = Observable::fromValue(value);
		Array<var> results;
		RxJUCECollectResults(o, results);
		
		RxJUCERequireResults(results, "Foo");
		
		WHEN("there is another Subscription and the Value is set twice") {
			RAIISubscription another = o.subscribe([&](String newValue) {
				results.add(newValue.toUpperCase());
			});
			
			value.setValue("Bar");
			RxJUCERunDispatchLoop();
			
			value.setValue("Baz");
			RxJUCERunDispatchLoop();
			
			THEN("both Subscriptions are notified") {
				for (auto s : {"Foo", "FOO", "BAR", "Bar", "BAZ", "Baz"})
					REQUIRE(results.contains(s));
			}
		}
		
		WHEN("there is another Observable and the Value is set once") {
			Observable another = Observable::fromValue(value);
			RxJUCECollectResults(another, results);
			
			THEN("both Observables emit an item") {
				RxJUCERequireResults(results, "Foo", "Foo");
			}
		}
		
		WHEN("there is another Value referring to the same Source") {
			Value another(value);
			Observable o2 = Observable::fromValue(another);
			RxJUCECollectResults(o2, results);
			
			THEN("both Values emit an item") {
				RxJUCERequireResults(results, "Foo", "Foo");
			}
		}
	}
}

TEST_CASE("A Slider Value can be observed",
		  "[Observable][Observable::fromValue]")
{
	GIVEN("a Slider with an initial value") {
		Slider slider;
		slider.setValue(7.6);
		Observable o = Observable::fromValue(slider.getValueObject());
		Array<var> results;
		RxJUCECollectResults(o, results);
		RxJUCERequireResults(results, 7.6);
		
		WHEN("the Slider value is changed once") {
			slider.setValue(0.45);
			RxJUCERunDispatchLoop();
			
			THEN("the Observable emits once") {
				RxJUCERequireResults(results, 7.6, 0.45);
			}
		}
		
		WHEN("the Slider value is changed rapidly") {
			for (double value : {3.41, 9.54, 4.67, 3.56})
				slider.setValue(value);
			
			RxJUCERunDispatchLoop();
			
			THEN("the Observable emits just once") {
				RxJUCERequireResults(results, 7.6, 3.56);
			}
		}
	}
}
