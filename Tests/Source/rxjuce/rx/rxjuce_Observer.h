/*
  ==============================================================================

    rxjuce_Observer.h
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

/**
	Retrieves items. You can call onNext to notify the Observer with a new item.
 
	@see BehaviorSubject, PublishSubject, Observable::create
 */
class Observer
{
public:
	/** Notifies the Observer with a new item. */
	void onNext(const juce::var& next) const;
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;
	
	friend class Observable;
	Observer(const std::shared_ptr<Impl>&);
	
	
	JUCE_LEAK_DETECTOR(Observer)
};

RXJUCE_NAMESPACE_END
