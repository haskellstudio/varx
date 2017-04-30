/*
  ==============================================================================

    rxjuce_Observed.h
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/


#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Subjects.h"

RXJUCE_NAMESPACE_BEGIN

/** If you get an error here, it means that you are trying to create an Observed<T> with an unsupported type T. */
template<typename Base, class Enable = void>
class Observed;

class ButtonForwarder : private juce::Button::Listener
{
public:
	ButtonForwarder(juce::Button& button);
	
	Observable clickedObservable() const;
	
	Observable buttonStateObservable() const;
	
private:
	PublishSubject clicked;
	BehaviorSubject buttonState;
	
	void buttonClicked(juce::Button *) override;
	void buttonStateChanged(juce::Button *button) override;
};

/**
	Adds Observable methods to a juce::Button or Button subclass.
 
	Usage:
 
		```cpp
		Observed<TextButton> myButton("My Button Text");
		myButton.clicked().subscribe([](var){ ... })
		```
 */
template<typename Base>
class Observed<Base, typename std::enable_if<std::is_base_of<juce::Button, Base>::value>::type> : public Base
{
public:
	template<typename... Args>
	Observed(Args&&... args)
	: Base(std::forward<Args>(args)...),
	  forwarder(*this)
	{}
	
	/**
		Returns an Observable that emits a value whenever the button is clicked. If you call triggerClick, the Observable emits asynchronously afterwards.
	 
		The emitted value is a void var (and can be ignored).
	 */
	Observable clickedObservable() const
	{
		return forwarder.clickedObservable();
	}
	
	/**
		Returns an Observable that emits a value synchronously whenever the button state changes.
	 
		The emitted value is a Button::ButtonState.
	 */
	Observable buttonStateObservable() const
	{
		return forwarder.buttonStateObservable();
	}
	
private:
	const ButtonForwarder forwarder;
};

template<>
class Observed<juce::Value> : public juce::Value
{
public:
	template<typename... Args>
	Observed(Args&&... args)
	: juce::Value(args...),
	  observable(Observable::fromValue(*this))
	{}
	
	Observable getObservable() const;
	
private:
	const Observable observable;
};


RXJUCE_NAMESPACE_END
