/*
  ==============================================================================

    varx_Scheduler_Impl.h
    Created: 2 May 2017 11:12:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

struct Scheduler::Impl
{
	typedef std::function<rxcpp::observable<juce::var>(const rxcpp::observable<juce::var>&)> Schedule;
	
	Impl(const Schedule& schedule);
	
	const Schedule schedule;
};


