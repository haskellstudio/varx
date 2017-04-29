/*
  ==============================================================================

    LifetimeWatcherPoolFixture.h
    Created: 29 Apr 2017 1:51:42pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_TestPrefix.h"

class LifetimeWatcherPoolFixture {
public:
	LifetimeWatcherPoolFixture();
	
	~LifetimeWatcherPoolFixture();
protected:
	LifetimeWatcherPool& pool;
	int getID();
	
private:
	static int uniqueID;
};
