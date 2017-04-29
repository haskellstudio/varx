/*
  ==============================================================================

    rxjuce_LifetimeWatcherPoolFixture.cpp
    Created: 29 Apr 2017 1:51:42pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_LifetimeWatcherPoolFixture.h"

LifetimeWatcherPoolFixture::LifetimeWatcherPoolFixture()
: pool(LifetimeWatcherPool::getInstance())
{
	REQUIRE(pool.getNumWatchers() == 0);
}

LifetimeWatcherPoolFixture::~LifetimeWatcherPoolFixture()
{
	pool.removeExpiredWatchers();
}

int LifetimeWatcherPoolFixture::getID() {
	return ++uniqueID;
}

int LifetimeWatcherPoolFixture::uniqueID = 0;
