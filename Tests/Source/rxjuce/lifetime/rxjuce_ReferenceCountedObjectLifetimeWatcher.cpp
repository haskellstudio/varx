/*
  ==============================================================================

    rxjuce_ReferenceCountedObjectLifetimeWatcher.cpp
    Created: 27 Apr 2017 7:14:12am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_ReferenceCountedObjectLifetimeWatcher.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

ReferenceCountedObjectLifetimeWatcher::ReferenceCountedObjectLifetimeWatcher(ReferenceCountedObject& object)
: object(&object)
{}

bool ReferenceCountedObjectLifetimeWatcher::isExpired() const
{
	return (object->getReferenceCount() == 1);
}

RXJUCE_NAMESPACE_END
