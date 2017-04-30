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

Subscription::Subscription(const std::function<bool()>& isSubscribed,
						   const std::function<void()>& unsubscribe)
: _isSubscribed(isSubscribed),
  _unsubscribe(unsubscribe) {}

void Subscription::unsubscribe() const
{
	_unsubscribe();
}

bool Subscription::isSubscribed() const
{
	return _isSubscribed();
}

RAIISubscription::RAIISubscription(Subscription&& subscription)
: Subscription(std::move(subscription)) {}

RAIISubscription::~RAIISubscription()
{
	unsubscribe();
}

RXJUCE_NAMESPACE_END
