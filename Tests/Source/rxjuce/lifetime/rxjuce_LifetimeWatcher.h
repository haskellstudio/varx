/*
  ==============================================================================

    rxjuce_LifetimeWatcher.h
    Created: 27 Apr 2017 7:13:13am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefixes.h"

RXJUCE_NAMESPACE_BEGIN

class LifetimeWatcher
{
public:
    virtual ~LifetimeWatcher() {}
	virtual bool isExpired() const = 0;
};

RXJUCE_NAMESPACE_END
