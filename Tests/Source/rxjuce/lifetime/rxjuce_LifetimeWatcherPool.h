/*
  ==============================================================================

    rxjuce_LifetimeWatcherPool.h
    Created: 27 Apr 2017 7:14:37am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

class LifetimeWatcher;

/**
 Keeps a list of LifetimeWatcher instances and periodically checks whether they have expired.
 
 An expired watcher is removed from the list and deleted. You can send a notification by implementing the destructor of a LifetimeWatcher subclass.
 
 @see LifetimeWatcher, ReferenceCountedObjectLifetimeWatcher, WeakReferenceLifetimeWatcher
 */
class LifetimeWatcherPool : private juce::Timer, private juce::DeletedAtShutdown
{
public:
	static LifetimeWatcherPool& getInstance();

	/** Adds a new watcher to the pool. The pool takes ownership of the watcher. */
	void add(std::unique_ptr<const LifetimeWatcher>&& watcher);

private:
	LifetimeWatcherPool();
	
	void timerCallback() override;
	
	std::multimap<const void*, std::unique_ptr<const LifetimeWatcher>> watchers;
	
	static LifetimeWatcherPool* instance;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LifetimeWatcherPool)
};

RXJUCE_NAMESPACE_END
