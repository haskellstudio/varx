/*
  ==============================================================================

    rxjuce_WeakReferenceLifetimeWatcher.h
    Created: 27 Apr 2017 7:13:48am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

template<typename T>
class WeakReferenceLifetimeWatcher : public LifetimeWatcher
{
public:
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