/*
  ==============================================================================

    rxjuce_Observer_Impl.h
    Created: 2 May 2017 9:02:15am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observer.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

using namespace juce;

struct Observer::Impl
{
	explicit Impl(const rxcpp::subscriber<var>& wrapped);
	
	const rxcpp::subscriber<var> wrapped;
};

RXJUCE_NAMESPACE_END
