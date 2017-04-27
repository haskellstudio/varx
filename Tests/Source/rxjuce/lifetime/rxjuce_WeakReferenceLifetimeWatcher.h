/*
  ==============================================================================

    rxjuce_WeakReferenceLifetimeWatcher.h
    Created: 27 Apr 2017 7:13:48am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_LifetimeWatcher.h"

RXJUCE_NAMESPACE_BEGIN

/**
 Watches a WeakReference to an object.
 
 If an instance of this is added to a LifetimeWatcherPool, it is deleted shortly after the watched object has been deleted.
 
 @see LifetimeWatcherPool
 */
template<typename T>
class WeakReferenceLifetimeWatcher : public LifetimeWatcher
{
public:
	/** Creates a new WeakReferenceLifetimeWatcher that watches a given weak reference. */
	WeakReferenceLifetimeWatcher(const juce::WeakReference<T>& ref)
	: ref(ref)
	{
	}
	
	const void* getAddress() const override
	{
		return ref.get();
	}

	bool isExpired(int) const override
	{
		return ref.wasObjectDeleted();
	}
private:
	const juce::WeakReference<T> ref;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WeakReferenceLifetimeWatcher)
};

RXJUCE_NAMESPACE_END
