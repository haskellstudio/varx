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
	/** Called when the LifetimeWatcher is removed from the LifetimeWatcherPool, shortly after it has expired. */
    virtual ~LifetimeWatcher() {}
	
	/** Should return whether the watched object has expired. */
	virtual bool isExpired() const = 0;
};

RXJUCE_NAMESPACE_END
