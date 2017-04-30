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
	Subscription(Subscription&&) = default;
	Subscription& operator=(Subscription&&) = default;
	
	void unsubscribe() const;
	
private:
	friend class Observable;
	explicit Subscription(const std::function<void()>&);
	
	std::function<void()> _unsubscribe;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscription)
};

class RAIISubscription
{
public:
	RAIISubscription(RAIISubscription&&) = default;
	RAIISubscription& operator=(RAIISubscription&&) = default;
	
	RAIISubscription(Subscription&& subscription);
	RAIISubscription& operator=(Subscription&&);
	
	~RAIISubscription();
	
private:
	Subscription subscription;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RAIISubscription)
};

RXJUCE_NAMESPACE_END
