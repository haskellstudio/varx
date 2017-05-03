/*
  ==============================================================================

    rxjuce_Scheduler.h
    Created: 3 May 2017 10:18:06pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

class Scheduler
{
public:
	static Scheduler messageThread();
	static Scheduler backgroundThread();
	static Scheduler newThread();
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;
	friend class Observable;
	Scheduler(const std::shared_ptr<Impl>&);
	
	JUCE_LEAK_DETECTOR(Scheduler)
};

RXJUCE_NAMESPACE_END
