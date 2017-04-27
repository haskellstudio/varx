/*
  ==============================================================================

    rxjuce_LifetimeWatcherPool.cpp
    Created: 27 Apr 2017 7:14:37am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_LifetimeWatcherPool.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

LifetimeWatcherPool& LifetimeWatcherPool::getInstance() {
	static LifetimeWatcherPool instance;
	return instance;
}

LifetimeWatcherPool::LifetimeWatcherPool() {}

void LifetimeWatcherPool::add(std::unique_ptr<const LifetimeWatcher>&& watcher)
{
	{
		// The timerCallback is called on the message thread. This may be called from a background thread, in which case the message manager must be locked.
		const MessageManagerLock lock(Thread::getCurrentThread());
		if (!lock.lockWasGained())
			return; // Some other thread is trying to kill this thread
		
		watchers.add(watcher.release());
	}
	startTimerHz(60);
}

void LifetimeWatcherPool::timerCallback()
{
	for (size_t i = 0; i < watchers.size();) {
		if (watchers[i]->isExpired())
			watchers.remove(i);
		else
			i += 1;
	}
	
	if (watchers.isEmpty())
		stopTimer();
}

RXJUCE_NAMESPACE_END
