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
	if (_unsubscribe)
		_unsubscribe();
}

RXJUCE_NAMESPACE_END
