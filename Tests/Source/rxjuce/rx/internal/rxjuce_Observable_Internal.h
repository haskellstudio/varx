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
	typedef Array<std::shared_ptr<void>> Sources;
	typedef std::shared_ptr<Internal> Ptr;
	
	Internal(const rxcpp::observable<var>& o);
	
	static Ptr fromRxCpp(const rxcpp::observable<var>& o);
	
	template<typename Transform, typename O, typename... Os>
	static Ptr combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		return fromRxCpp(observable.internal->o.combine_latest(transform, observables.internal->o...));
	}
	
	const rxcpp::observable<var> o;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

RXJUCE_NAMESPACE_END
