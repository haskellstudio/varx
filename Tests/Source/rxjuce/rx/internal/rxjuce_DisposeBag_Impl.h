/*
  ==============================================================================

    rxjuce_DisposeBag_Impl.h
    Created: 13 May 2017 3:08:51pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_DisposeBag.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

struct DisposeBag::Impl
{
	const rxcpp::composite_subscription wrapped;
};

RXJUCE_NAMESPACE_END
