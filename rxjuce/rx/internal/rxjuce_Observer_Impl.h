/*
  ==============================================================================

    rxjuce_Observer_Impl.h
    Created: 2 May 2017 9:02:15am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

struct Observer::Impl
{
	explicit Impl(const rxcpp::subscriber<var>& wrapped);
	
	const rxcpp::subscriber<var> wrapped;
};


