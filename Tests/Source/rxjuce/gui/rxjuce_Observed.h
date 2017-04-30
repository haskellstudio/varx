/*
  ==============================================================================

    rxjuce_Observed.h
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/


#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_BehaviorSubject.h"
#include "rxjuce_Observable.h"
#include "rxjuce_PublishSubject.h"

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
	: Base(std::forward<Args>(args)...),
	  _stateChanged(Base::getState())
	{
		this->addListener(this);
	}
	
	/**
		Returns an Observable that emits a value whenever the button is clicked. If you call triggerClick, the Observable emits asynchronously afterwards.
	 
		The emitted value is an empty var (and can be ignored).
	 */
	Observable clicked() const
	{
		return _clicked.getObservable();
	}
	
	/**
		Returns an Observable that emits a value whenever the button state changes.
	 
		The emitted value is a ButtonState.
	 */
	Observable stateChanged() const
	{
		return _stateChanged.getObservable();
	}
	
private:
	PublishSubject _clicked;
	BehaviorSubject _stateChanged;
	
	void buttonClicked(juce::Button *) override
	{
		_clicked.onNext(juce::var());
	}
	
	void buttonStateChanged(juce::Button *) override
	{
		_stateChanged.onNext(juce::var(Base::getState()));
	}
};

template<>
class Observed<juce::Value> : public juce::Value, private juce::Value::Listener
{
public:
	template<typename... Args>
	Observed(Args&&... args)
	: juce::Value(args...),
	  _value(juce::Value::getValue())
	{
		addListener(this);
	}
	
	Observable getObservable() const
	{
		return _value.getObservable();
	}
	
private:
	BehaviorSubject _value;
	
	void valueChanged(Value &value) override
	{
		_value.onNext(value);
	}
};


RXJUCE_NAMESPACE_END
