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

namespace detail {
	class ButtonForwarder : private juce::Button::Listener
	{
	public:
		explicit ButtonForwarder(juce::Button& button);
		
		Observable clickedObservable() const;
		
		Observable buttonStateObservable() const;
		
	private:
		PublishSubject clicked;
		BehaviorSubject buttonState;
		
		void buttonClicked(juce::Button *) override;
		void buttonStateChanged(juce::Button *) override;
	};
}

// If you get an error here, it means that you are trying to create an Observed<T> with an unsupported type T.
template<typename T, class Enable = void>
class Observed;

/**
	Adds Observables to a juce::Button or Button subclass.
 
	The Observables emit items whenever the Button is clicked or changes its ButtonState.
 
	Usage:
 
		Observed<TextButton> myButton("My Button Text");
		myButton.clickedObservable().subscribe([](var){ ... })
 */
template<typename T>
class Observed<T, typename std::enable_if<std::is_base_of<juce::Button, T>::value>::type> : public T
{
public:
	/**
		Creates a new instance. Takes the same parameters as your Button's constructor(s).
	 */
	template<typename... Args>
	Observed(Args&&... args)
	: T(std::forward<Args>(args)...),
	  forwarder(*this)
	{}
	
	/**
		Returns an Observable that emits a value whenever the button is clicked. If you call triggerClick, the Observable emits asynchronously afterwards.
	 
		The emitted value is a void var (and can be ignored).
	 */
	Observable clickedObservable() const { return forwarder.clickedObservable(); }
	
	/**
		Returns an Observable that emits a value synchronously whenever the button state changes.
	 
		The emitted value is a Button::ButtonState.
	 */
	Observable buttonStateObservable() const { return forwarder.buttonStateObservable(); }
	
private:
	const detail::ButtonForwarder forwarder;
};


/**
	Adds an Observable to a juce::Value. The Observable emits whenever the Value is changed.
 
	@see getObservable()
 */
template<>
class Observed<juce::Value> : public juce::Value
{
public:
	/**
		Creates a new instance. Takes the same parameters as the juce::Value constructor(s).
	 */
	template<typename... Args>
	Observed(Args&&... args)
	: juce::Value(args...),
	  observable(Observable::fromValue(*this)) {}
	
	/**
		Returns an Observable that emits an item asynchronously whenever the Value changes, until the Observed<Value> is destroyed.
	 */
	Observable getObservable() const;
	
private:
	const Observable observable;
};


RXJUCE_NAMESPACE_END
