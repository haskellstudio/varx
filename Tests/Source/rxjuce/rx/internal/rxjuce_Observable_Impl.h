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

struct Observable::Impl
{
public:
	Impl(const rxcpp::observable<var>& wrapped = rxcpp::observable<var>());
	
	static std::shared_ptr<Impl> fromRxCpp(const rxcpp::observable<var>& wrapped);
	
	static std::shared_ptr<Impl> fromValue(const Value& value);
	
	template<typename Transform, typename... Os>
	std::shared_ptr<Impl> combineLatest(Transform&& transform, Os&&... observables)
	{
		return fromRxCpp(wrapped.combine_latest(transform, observables.impl->wrapped...));
	}
	
	template<typename... Os>
	std::shared_ptr<Impl> merge(Os&&... observables)
	{
		return fromRxCpp(wrapped.merge(observables.impl->wrapped...));
	}
	
	rxcpp::observable<var> wrapped;
	
private:
	friend class PublishSubject;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Impl)
};

RXJUCE_NAMESPACE_END
