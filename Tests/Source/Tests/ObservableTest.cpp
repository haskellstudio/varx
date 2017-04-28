/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

SCENARIO("Value Observable can emit after Value is destroyed",
		 "[Observable][Observable::fromValue]")
{
	GIVEN("an Observable from a Value") {
		auto value = std::make_shared<Value>(15);
		Observable o = Observable::fromValue(*value);
		RxJUCECollectResults(o, results);
		RxJUCERequireResults(results, 15);
		
		WHEN("the Value is set to a new value and destroyed immediately") {
			value->setValue(17);
			value.reset();
			RxJUCERunDispatchLoop();
			
			THEN("the Observable still emits an item") {
				RxJUCERequireResults(results, 15, 17);
			}
		}
	}
}

SCENARIO("Observable emits as long as ValueSource is not destroyed",
		 "[Observable][Observable::fromValue]")
{
	GIVEN("an Observable from a Value which has an initial item") {
		auto value = std::make_shared<Value>("Initial");
		Observable o = Observable::fromValue(*value);
		RxJUCECollectResults(o, results);
		RxJUCERequireResults(results, "Initial");
		
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
	}
}

TEST_CASE("A Slider Value can be observed",
		  "[Observable][Observable::fromValue]")
{
	GIVEN("a Slider with an initial value") {
		Slider slider;
		slider.setValue(7.6);
		Observable o = Observable::fromValue(slider.getValueObject());
		RxJUCECollectResults(o, results);
		RxJUCERequireResults(results, 7.6);
		
		WHEN("the Slider value changes") {
			slider.setValue(0.45);
			RxJUCERunDispatchLoop();
			
			THEN("the Observable emits") {
				RxJUCERequireResults(results, 7.6, 0.45);
			}
		}
	}
}

TEST_CASE("A Value can have 2 Observables",
		  "[Observable][Observable::fromValue]")
{
	GIVEN("a Value with 2 Observables") {
		Value value(3.54);
		Observable o1 = Observable::fromValue(value);
		Observable o2 = Observable::fromValue(value);
		RxJUCECollectResults(o1, o1Results);
		RxJUCECollectResults(o2, o2Results);
		
		RxJUCERequireResults(o1Results, 3.54);
		RxJUCERequireResults(o2Results, 3.54);
		
		WHEN("the Value changes") {
			value.setValue(76.4);
			RxJUCERunDispatchLoop();
			
			THEN("both Observables emit an item") {
				RxJUCERequireResults(o1Results, 3.54, 76.4);
				RxJUCERequireResults(o2Results, 3.54, 76.4);
			}
		}
	}
}

TEST_CASE("A Value Observable can have 2 Subscriptions",
		  "[Observable][Observable::fromValue]")
{
	GIVEN("a Value Observable with 2 Subscriptions") {
		Value value("Foo");
		Observable o = Observable::fromValue(value);
		Array<var> allResults;
		
		RAIISubscription s1 = o.subscribe([&](var newValue) {
			allResults.add(newValue);
		});
		
		RAIISubscription s2 = o.subscribe([&](var newValue) {
			allResults.add(newValue.toString().toUpperCase());
		});
		
		WHEN("the Value changes multiple times") {
			value.setValue("Bar");
			RxJUCERunDispatchLoop();
			
			value.setValue("Baz");
			RxJUCERunDispatchLoop();
			
			THEN("both Subscriptions are notified") {
				RxJUCERequireResults(allResults, "Foo", "FOO", "BAR", "Bar", "BAZ", "Baz");
			}
		}
	}
}
