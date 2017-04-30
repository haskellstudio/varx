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
#include "rxjuce_BehaviorSubject.h"

RXJUCE_NAMESPACE_BEGIN

/** If you get an error here, it means that you are trying to create an Observe<T> with an unsupported type T. */
template<typename Base, class Enable = void>
class Observed;

/**
	Adds Observable methods to a juce::Button or Button subclass.
 
	Usage:
 
		```cpp
		Observed<TextButton> myButton("My Button Text");
		myButton.clickedObservable().subscribe([](var){ ... })
		```
 */
template<typename Base>
class Observed<Base, typename std::enable_if<std::is_base_of<juce::Button, Base>::value>::type> : public Base, private juce::Button::Listener
{
public:
	template<typename... Args>
	Observed(Args&&... args)
	: Base(std::forward<Args>(args)...)
	{
		addListener(this);
	}
	
	/**
		Returns an Observable that emits a value whenever the button is clicked, until the button is destroyed.
	 
		The emitted value is an empty var (and can be ignored).
	 */
	Observable clickedObservable() const
	{
		return _clicked.getObservable();
	}
	
	/**
		Returns an Observable that emits a value whenever the button state changes, until the button is destroyed.
	 
		The emitted value is a ButtonState.
	 */
	Observable stateChanged() const
	{
		return _stateChanged.getObservable();
	}
	
private:
	BehaviorSubject _clicked;
	BehaviorSubject _stateChanged;
};


RXJUCE_NAMESPACE_END
