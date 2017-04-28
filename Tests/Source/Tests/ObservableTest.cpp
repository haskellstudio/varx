/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

SCENARIO("As long as one Value points to a source, it emits values.",
		 "[Observable][Observable::fromValue]")
{
	GIVEN("a Value") {
		auto value = std::make_shared<Value>();
		
		WHEN("a subscription is made to its Observable") {
			RxJUCECollectResults(Observable::fromValue(*value), results);
			
			AND_WHEN("a copy of the value is made") {
				auto copy = std::make_shared<Value>(*value);
				
				AND_WHEN("the original value is destroyed") {
					value = nullptr;
					
					THEN("the subscription still receives values") {
						copy->setValue(17);
						RxJUCERunDispatchLoop;
						copy->setValue("Foo");
						RxJUCERunDispatchLoop;
						
						RxJUCERequireResults(var(), var(17), var("Foo"));
					}
				}
				
				AND_WHEN("a value is set on the copy immediately before destroying it") {
					copy->setValue("Bar");
					copy = nullptr;
					
					THEN("the subscription still receives the value asynchronously") {
						RxJUCERunDispatchLoop;
						RxJUCERequireResults(var(), var("Bar"));
					}
				}
			}
		}
	}
}
