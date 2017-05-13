/*
  ==============================================================================

    rxjuce_Disposable_Impl.h
    Created: 2 May 2017 9:16:56am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Disposable.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

struct Disposable::Impl
{
	Impl(const rxcpp::subscription& wrapped);
	
	const rxcpp::subscription wrapped;
};

RXJUCE_NAMESPACE_END
