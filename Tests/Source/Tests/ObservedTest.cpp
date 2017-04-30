/*
  ==============================================================================

    ObservedTest.cpp
    Created: 28 Apr 2017 8:12:02pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observed.h"


TEST_CASE("Observed<Button> stateChanged")
{
	Observed<TextButton> button("Click Here");
	Array<var> results;
	RxJUCECollectItems(button.buttonStateObservable(), results);
	
	IT("emits the normal state on subscribe") {
		RxJUCERequireResults(results, Button::ButtonState::buttonNormal);
	}
	
	IT("emits items synchronously when the Button state changes") {
		button.setState(Button::ButtonState::buttonDown);
		
		RxJUCERequireResults(results,
							 Button::ButtonState::buttonNormal,
							 Button::ButtonState::buttonDown);
		
		button.setState(Button::ButtonState::buttonNormal);
		button.setState(Button::ButtonState::buttonOver);
		
		RxJUCERequireResults(results,
							 Button::ButtonState::buttonNormal,
							 Button::ButtonState::buttonDown,
							 Button::ButtonState::buttonNormal,
							 Button::ButtonState::buttonOver);
	}
}


TEST_CASE("Observed<Button> clicked")
{
	Observed<TextButton> button("Click Here");
	Array<var> results;
	RxJUCECollectItems(button.clickedObservable(), results);
	
	IT("doesn't emit an item on subscribe") {
		REQUIRE(results.isEmpty());
	}
	
	IT("emits void vars asynchronously when the Button is clicked") {
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, var());
		
		button.triggerClick();
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, var(), var(), var());
	}
}


TEST_CASE("Observed<Value>")
{
	Observed<Value> value("Initial");
	Array<var> results;
	RxJUCECollectItems(value.getObservable(), results);
	
	IT("emits items asynchronously when the Value changes") {
		value.setValue("Second");
		RxJUCERunDispatchLoop();
		value.setValue("Third");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial", "Second", "Third");
	}
}
