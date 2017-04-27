/*
  ==============================================================================

    rxjuce_WeakReferenceLifetimeWatcher.h
    Created: 27 Apr 2017 7:13:48am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

/**
 Watches an object containing a WeakReference::Master.
 
 If an instance of this is added to a LifetimeWatcherPool, it is deleted shortly after the watched object has been deleted.
 
 @see LifetimeWatcherPool
 */
template<typename T>
class WeakReferenceLifetimeWatcher : public LifetimeWatcher
{
public:
	/** Creates a new WeakReferenceLifetimeWatcher that watches a given object. */
	WeakReferenceLifetimeWatcher(const T& object)
	: ref(&object)
	{}

	bool isExpired() const override
	{
		return ref.wasObjectDeleted();
	}
private:
	const WeakReference<T> ref;
};
