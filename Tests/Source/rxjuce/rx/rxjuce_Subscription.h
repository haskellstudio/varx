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
	bool isSubscribed() const;
	
private:
	friend class Observable;
	explicit Subscription(const std::function<bool()>&, const std::function<void()>&);
	
	const std::function<bool()> _isSubscribed;
	const std::function<void()> _unsubscribe;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscription)
};

class RAIISubscription : public Subscription
{
public:
	RAIISubscription(Subscription&& subscription);
	RAIISubscription(RAIISubscription&&) = default;
	~RAIISubscription();
	
	RAIISubscription& operator=(RAIISubscription&&) = default;
	
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RAIISubscription)
};

RXJUCE_NAMESPACE_END
