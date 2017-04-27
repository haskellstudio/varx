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
#include "rxjuce_Subscriber.h"
#include "rxjuce_WeakReferenceLifetimeWatcher.h"
#include "rxjuce_LifetimeWatcherPool.h"

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
class Observed<Base, typename std::enable_if<std::is_base_of<juce::Button, Base>::value>::type> : public Base
{
public:
	using Base::Base;
	
	~Observed()
	{
		masterReference.clear();
	}
	
	/**
		Returns an Observable that emits a value whenever the button is clicked, until the button is destroyed.
	 
		The emitted value is an empty var (and can be ignored).
	 */
	Observable clickedObservable()
	{
		class ClickedForwarder : public ButtonForwarder
		{
		public:
			using ButtonForwarder::ButtonForwarder;
			
			void buttonClicked(juce::Button *) override
			{
				this->subscriber.onNext(juce::var());
			}
		};
		
		return createObservable<ClickedForwarder>();
	}
	
	/**
		Returns an Observable that emits a value whenever the button state changes, until the button is destroyed.
	 
		The emitted value is a ButtonState.
	 */
	Observable stateChangedObservable()
	{
		class StateChangedForwarder : public ButtonForwarder
		{
		public:
			using ButtonForwarder::ButtonForwarder;
			
			void buttonClicked(juce::Button *) override
			{}
			
			void buttonStateChanged(juce::Button *button) override
			{
				this->subscriber.onNext(juce::var(button->getState()));
			}
		};
		
		return createObservable<StateChangedForwarder>();
	}
	
private:
	typedef Observed<Base> Self;
	typedef juce::WeakReference<Self> WeakRef;
	
	class ButtonForwarder : public juce::Button::Listener, public WeakReferenceLifetimeWatcher<Self>
	{
	public:
		ButtonForwarder(const WeakRef& ref, Subscriber subscriber)
		: WeakReferenceLifetimeWatcher<Self>(ref),
		  subscriber(subscriber)
		{}
		
	protected:
		const Subscriber subscriber;
	};
	
	template<typename Forwarder>
	Observable createObservable()
	{
		WeakRef weakSelf(this);
		
		return Observable::create([weakSelf](Subscriber subscriber) mutable {
			if (weakSelf.wasObjectDeleted())
				return;
			
			std::unique_ptr<Forwarder> forwarder(new Forwarder(weakSelf, subscriber));
			weakSelf->addListener(forwarder.get());
			LifetimeWatcherPool::getInstance().add(std::move(forwarder));
		});
	}
	
	typename WeakRef::Master masterReference;
	friend class juce::WeakReference<Self>;
};


RXJUCE_NAMESPACE_END
