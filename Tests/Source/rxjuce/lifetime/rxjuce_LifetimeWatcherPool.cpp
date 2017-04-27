/*
  ==============================================================================

    rxjuce_LifetimeWatcherPool.cpp
    Created: 27 Apr 2017 7:14:37am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_LifetimeWatcherPool.h"

#include "rxjuce_LifetimeWatcher.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

LifetimeWatcherPool& LifetimeWatcherPool::getInstance()
{
	if (!instance)
		instance = new LifetimeWatcherPool();
	
	return *instance;
}

LifetimeWatcherPool::LifetimeWatcherPool()
{}

LifetimeWatcherPool* LifetimeWatcherPool::instance = nullptr;

void LifetimeWatcherPool::add(std::unique_ptr<const LifetimeWatcher>&& watcher)
{
	{
		// The timerCallback is called on the message thread. This may be called from a background thread, in which case the message manager must be locked.
		const MessageManagerLock lock(Thread::getCurrentThread());
		if (!lock.lockWasGained())
			return; // Some other thread is trying to kill this thread
		
		watchers.insert(std::make_pair(watcher->getAddress(), std::move(watcher)));
	}
	
	startTimerHz(60);
}

void LifetimeWatcherPool::timerCallback()
{
	for (auto it = watchers.begin(); it != watchers.end(); it++) {
		if (it->second->isExpired(watchers.count(it->first))) {
			const auto range = watchers.equal_range(it->first);
			it = watchers.erase(range.first, range.second);
			if (it == watchers.end())
				break;
		}
	}
	
	if (watchers.empty())
		stopTimer();
}

RXJUCE_NAMESPACE_END
