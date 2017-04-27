/*
  ==============================================================================

    rxjuce_ReferenceCountedObjectLifetimeWatcher.h
    Created: 27 Apr 2017 7:14:12am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_LifetimeWatcher.h"

RXJUCE_NAMESPACE_BEGIN

class ReferenceCountedObjectLifetimeWatcher : public LifetimeWatcher
{
public:
	ReferenceCountedObjectLifetimeWatcher(juce::ReferenceCountedObject& object);

	bool isExpired() const override;
	
private:
	const juce::ReferenceCountedObjectPtr<juce::ReferenceCountedObject> object;
};

RXJUCE_NAMESPACE_END
