/*
  ==============================================================================

    varx_Observer_Impl.cpp
    Created: 2 May 2017 9:02:15am
    Author:  Martin Finke

  ==============================================================================
*/

#include "varx_Observer_Impl.h"

Observer::Impl::Impl(const rxcpp::subscriber<var>& wrapped)
: wrapped(wrapped) {}


