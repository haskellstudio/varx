/*
  ==============================================================================

    rxjuce_Subscription.h
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

RXJUCE_NAMESPACE_BEGIN

class Subscription
{
public:
	Subscription(Subscription&& other);
	Subscription& operator=(Subscription&& other);
	
	void unsubscribe() const;
	
private:
	friend class Observable;
	Subscription(const std::function<void()>& unsubscribe);
	
	std::function<void()> _unsubscribe;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscription)
};

RXJUCE_NAMESPACE_END
