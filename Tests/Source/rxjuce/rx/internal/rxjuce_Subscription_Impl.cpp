/*
  ==============================================================================

    rxjuce_Subscription_Impl.cpp
    Created: 2 May 2017 9:16:56am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subscription_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Subscription::Impl::Impl(const rxcpp::subscription& wrapped)
: wrapped(wrapped) {}

RXJUCE_NAMESPACE_END
