//
//  rxjuce_Observable_Internal.h
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

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
	
	static Ptr fromRxCpp(const rxcpp::observable<var>& o);
	
	template<typename Transform, typename O, typename... Os>
	static Ptr combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		return fromRxCpp(observable.internal->o.combine_latest(transform, observables.internal->o...));
	}
	
	template<typename T>
	static Ptr range(var first, var last, int step)
	{
		return fromRxCpp(rxcpp::observable<>::range<T>(first, last, step).map(VariantConverter<T>::toVar));
	}
	
	const rxcpp::observable<var> o;
	
private:
	Internal(const rxcpp::observable<var>& o);
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

RXJUCE_NAMESPACE_END
