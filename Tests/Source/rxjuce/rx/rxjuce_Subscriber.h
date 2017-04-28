/*
  ==============================================================================

    rxjuce_Subscriber.h
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

class Subscriber
{
public:
	void onNext(const juce::var& next) const;
	
private:
	friend class Observable;
	explicit Subscriber(const std::function<void(const juce::var&)>& onNext);
	
	std::function<void(const juce::var&)> _onNext;
	
	JUCE_LEAK_DETECTOR(Subscriber)
};

RXJUCE_NAMESPACE_END
