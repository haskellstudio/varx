/*
  ==============================================================================

    varx_DisposeBag_Impl.h
    Created: 13 May 2017 3:08:51pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

struct DisposeBag::Impl
{
	const rxcpp::composite_subscription wrapped;
};


