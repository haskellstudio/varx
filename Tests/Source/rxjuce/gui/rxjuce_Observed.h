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
	template<typename T>
	using IsJUCEButton = typename std::enable_if<std::is_base_of<juce::Button, T>::value>::type;
	
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
class Observed<T, detail::IsJUCEButton<T>> : public T
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
	/** Creates a new instance. Has the same behavior as the juce::Value equivalent. */
	///@{
	Observed();
	Observed(const Value& other);
	explicit Observed(const juce::var& initialValue);
	///@}
	
	/** Returns the current value. Has the same behavior as the juce::Value equivalent. */
	operator juce::var() const;
	
	/** Sets a new value. This is the same as calling Observed<Value>::setValue. */
	Observed& operator=(const juce::var& newValue);
	
	/**
		Returns an Observable that emits an item asynchronously whenever the Value changes, until the Observed<Value> is destroyed.
	 */
	Observable getObservable() const;
	
private:
	const Observable observable;
	
	Observed& operator=(const Observed&) = delete;
};


RXJUCE_NAMESPACE_END
