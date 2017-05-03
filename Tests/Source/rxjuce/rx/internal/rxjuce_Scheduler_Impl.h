/*
  ==============================================================================

    rxjuce_Scheduler_Impl.h
    Created: 2 May 2017 11:12:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Scheduler.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

struct Scheduler::Impl
{
	typedef std::function<rxcpp::observable<juce::var>(const rxcpp::observable<juce::var>&)> Schedule;
	
	Impl(const Schedule& schedule);
	
	const Schedule schedule;
};

RXJUCE_NAMESPACE_END
