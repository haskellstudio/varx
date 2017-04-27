/*
  ==============================================================================

    rxjuce_LifetimeWatcher.h
    Created: 27 Apr 2017 7:13:13am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

/**
 Base class for lifetime watcher objects.
 
 @see LifetimeWatcherPool, ReferenceCountedObjectLifetimeWatcher, WeakReferenceLifetimeWatcher
 */
class LifetimeWatcher
{
public:
	LifetimeWatcher() {}
	
	/** Called when the LifetimeWatcher is removed from the LifetimeWatcherPool, shortly after it has expired. */
    virtual ~LifetimeWatcher() {}
	
	/** Should return the address of the referenced object. Used to group multiple LifetimeWatchers which watch the same object. */
	virtual const void* getAddress() const = 0;
	
	/** Should return whether the object is referenced by more than the provided number of LifetimeWatchers. */
	virtual bool isExpired(int numLifetimeWatchers) const = 0;
};

RXJUCE_NAMESPACE_END
