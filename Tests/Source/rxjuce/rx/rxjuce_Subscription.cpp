/*
  ==============================================================================

    rxjuce_Subscription.cpp
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subscription.h"

#include "rxjuce_Subscription_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Subscription::Subscription(const std::shared_ptr<Impl>& impl)
: impl(impl) {}

void Subscription::unsubscribe() const
{
	impl->wrapped.unsubscribe();
}

bool Subscription::isSubscribed() const
{
	return impl->wrapped.is_subscribed();
}


ScopedSubscription::ScopedSubscription(Subscription&& subscription)
: Subscription(std::move(subscription)) {}

ScopedSubscription::~ScopedSubscription()
{
	unsubscribe();
}

RXJUCE_NAMESPACE_END
