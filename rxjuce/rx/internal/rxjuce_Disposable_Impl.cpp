/*
  ==============================================================================

    rxjuce_Disposable_Impl.cpp
    Created: 2 May 2017 9:16:56am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Disposable_Impl.h"

Disposable::Impl::Impl(const rxcpp::subscription& wrapped)
: wrapped(wrapped) {}


