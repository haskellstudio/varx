/*
  ==============================================================================

    rxjuce_Observable_Impl.h
    Created: 27 Apr 2017 7:11:30am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observable.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

using namespace juce;

class Observable::Impl
{
public:
	Impl(const rxcpp::observable<var>& wrapped = rxcpp::observable<var>());
	
	static std::shared_ptr<Impl> fromRxCpp(const rxcpp::observable<var>& wrapped);
	
	static std::shared_ptr<Impl> fromValue(const Value& value);
	
	template<typename Transform, typename... Os>
	std::shared_ptr<Impl> combineLatest(Transform&& transform, Os... observables)
	{
		auto combined = wrapped.combine_latest(transform, observables.impl->wrapped...);
		return fromRxCpp(combined);
	}
	
	rxcpp::observable<var> wrapped;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Impl)
};

RXJUCE_NAMESPACE_END
