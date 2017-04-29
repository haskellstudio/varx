/*
  ==============================================================================

    rxjuce_Subscription.cpp
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subscription.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Subscription::Subscription(const std::function<void()>& unsubscribe)
: _unsubscribe(unsubscribe)
{}

void Subscription::unsubscribe() const
{
	_unsubscribe();
}

RAIISubscription::RAIISubscription(Subscription&& subscription)
: subscription(std::move(subscription))
{}

RAIISubscription::~RAIISubscription()
{
	subscription.unsubscribe();
}

RAIISubscription& RAIISubscription::operator=(Subscription&& other)
{
	subscription = std::move(other);
	return *this;
}

RXJUCE_NAMESPACE_END
