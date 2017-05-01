/*
  ==============================================================================

    ObservedTest.cpp
    Created: 28 Apr 2017 8:12:02pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observed.h"


TEST_CASE("Observed<Button> stateChanged",
		  "[Observed<Button>]")
{
	Observed<TextButton> button("Click Here");
	Array<var> items;
	RxJUCECollectItems(button.buttonStateObservable(), items);
	
	IT("emits the normal state on subscribe") {
		RxJUCERequireItems(items, Button::ButtonState::buttonNormal);
	}
	
	IT("emits items synchronously when the Button state changes") {
		button.setState(Button::ButtonState::buttonDown);
		
		RxJUCECheckItems(items,
						 Button::ButtonState::buttonNormal,
						 Button::ButtonState::buttonDown);
		
		button.setState(Button::ButtonState::buttonNormal);
		button.setState(Button::ButtonState::buttonOver);
		
		RxJUCERequireItems(items,
						   Button::ButtonState::buttonNormal,
						   Button::ButtonState::buttonDown,
						   Button::ButtonState::buttonNormal,
						   Button::ButtonState::buttonOver);
	}
}


TEST_CASE("Observed<Button> clicked",
		  "[Observed<Button>]")
{
	Observed<TextButton> button("Click Here");
	Array<var> items;
	RxJUCECollectItems(button.clickedObservable(), items);
	
	IT("doesn't emit an item on subscribe") {
		REQUIRE(items.isEmpty());
	}
	
	IT("emits void vars asynchronously when the Button is clicked") {
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCECheckItems(items, var());
		
		button.triggerClick();
		button.triggerClick();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, var(), var(), var());
	}
}


TEST_CASE("Observed<Value>",
		  "[Observed<Value>]")
{
	Observed<Value> value("Initial");
	Array<var> items;
	RxJUCECollectItems(value.getObservable(), items);
	
	IT("emits items asynchronously when the Value changes") {
		value.setValue("Second");
		RxJUCERunDispatchLoop();
		value.setValue("Third");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial", "Second", "Third");
	}
}
