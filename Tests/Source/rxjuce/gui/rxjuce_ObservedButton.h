/*
  ==============================================================================

    rxjuce_ObservedButton.h
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

template<typename Base>
class ObservedButton : public Base
{
	typedef ObservedButton<Base> Self;
	
public:
	using Base::Base;
	
	~ObservedButton()
	{
		masterReference.clear();
	}
	
	Observable clickedObservable()
	{
		class Forwarder : public Base::Listener, public WeakReferenceLifetimeWatcher<Self>
		{
		public:
			Forwarder(const juce::WeakReference<Self>& ref, Subscriber subscriber)
			: WeakReferenceLifetimeWatcher<Self>(ref),
			  subscriber(subscriber)
			{}
			
			void buttonClicked(juce::Button *) override
			{
				subscriber.onNext(juce::var());
			}
			
		private:
			const Subscriber subscriber;
		};
		
		juce::WeakReference<Self> weakSelf(this);
		
		return Observable::create([weakSelf](Subscriber subscriber) mutable {
			if (weakSelf.wasObjectDeleted())
				return;
			
			std::unique_ptr<Forwarder> forwarder(new Forwarder(weakSelf, subscriber));
			weakSelf->addListener(forwarder.get());
			LifetimeWatcherPool::getInstance().add(std::move(forwarder));
		});
	}
	
private:
	typename juce::WeakReference<Self>::Master masterReference;
	friend class juce::WeakReference<Self>;
};


RXJUCE_NAMESPACE_END
