/*
  ==============================================================================

    rxjuce_ReferenceCountedObjectLifetimeWatcher.h
    Created: 27 Apr 2017 7:14:12am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_LifetimeWatcher.h"

RXJUCE_NAMESPACE_BEGIN

/**
 Watches a ReferenceCountedObject.
 
 If an instance of this is added to a LifetimeWatcherPool, it will be deleted shortly after there are no other references to the watched object.
 
 @see LifetimeWatcherPool
 */
class ReferenceCountedObjectLifetimeWatcher : public LifetimeWatcher
{
public:
	/** Creates a new ReferenceCountedObjectLifetimeWatcher that watches a given object. */
	ReferenceCountedObjectLifetimeWatcher(juce::ReferenceCountedObject *object);

	bool isExpired() const override;
	
private:
	const juce::ReferenceCountedObjectPtr<juce::ReferenceCountedObject> object;
};

RXJUCE_NAMESPACE_END
