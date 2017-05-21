/*
  ==============================================================================

    varx_Disposable_Impl.h
    Created: 2 May 2017 9:16:56am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

struct Disposable::Impl
{
	Impl(const rxcpp::subscription& wrapped);
	
	const rxcpp::subscription wrapped;
};


