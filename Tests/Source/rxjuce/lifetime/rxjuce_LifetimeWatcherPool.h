/*
  ==============================================================================

    rxjuce_LifetimeWatcherPool.h
    Created: 27 Apr 2017 7:14:37am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_LifetimeWatcher.h"

RXJUCE_NAMESPACE_BEGIN

class LifetimeWatcherPool : private juce::Timer
{
public:
	static LifetimeWatcherPool& getInstance();

	void add(const LifetimeWatcher *watcher);

private:
	LifetimeWatcherPool();
	
	void timerCallback() override;
	
	juce::OwnedArray<const LifetimeWatcher> watchers;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LifetimeWatcherPool)
};

RXJUCE_NAMESPACE_END
