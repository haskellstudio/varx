/*
  ==============================================================================

    rxjuce_Observable_Internal.h
    Created: 27 Apr 2017 7:11:30am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Observable.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

RXJUCE_NAMESPACE_BEGIN

using namespace juce;

class Observable::Internal
{
public:
	Internal();
	
	static std::shared_ptr<Internal> fromRxCpp(const rxcpp::observable<var>& o);
	
	static std::shared_ptr<Internal> fromValue(const Value& value);
	
	template<typename Transform, typename O, typename... Os>
	static std::shared_ptr<Internal> combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		auto combined = observable.internal->o.combine_latest(transform, observables.internal->o...);
		return fromRxCpp(combined);
	}
	
	rxcpp::observable<var> o;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

RXJUCE_NAMESPACE_END
