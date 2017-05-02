/*
  ==============================================================================

    rxjuce_Observer_Impl.cpp
    Created: 2 May 2017 9:02:15am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observer_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observer::Impl::Impl(const rxcpp::subscriber<var>& wrapped)
: wrapped(wrapped) {}

RXJUCE_NAMESPACE_END
