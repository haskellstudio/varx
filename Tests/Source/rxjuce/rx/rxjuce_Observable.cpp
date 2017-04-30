/*
  ==============================================================================

    rxjuce_Observable.cpp
    Created: 27 Apr 2017 7:07:33am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observable.h"

#include "rxjuce_Observable_Internal.h"

#include "rxjuce_Subscriber.h"

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observable Observable::fromValue(Value value)
{
	return Internal::fromValue(value);
}

Observable::Observable(const shared_ptr<Internal>& internal)
:	internal(internal)
{}

Subscription Observable::subscribe(const std::function<void(const var&)>& onNext) const
{
	const auto subscription = internal->o.subscribe(onNext);
	
	return Subscription([subscription]() {
		subscription.unsubscribe();
	});
}

Observable Observable::just(var value)
{
	return Internal::fromRxCpp(rxcpp::observable<>::just(value));
}

Observable Observable::range(juce::Range<int> range, int step)
{
	auto o = rxcpp::observable<>::range<int>(range.getStart(), range.getEnd(), step);
	
	return Internal::fromRxCpp(o.map(juce::VariantConverter<int>::toVar));
}
Observable Observable::range(juce::Range<double> range, int step)
{
	auto o = rxcpp::observable<>::range<double>(range.getStart(), range.getEnd(), step);
	
	return Internal::fromRxCpp(o.map(juce::VariantConverter<double>::toVar));
}

Observable Observable::create(const std::function<void(Subscriber)>& onSubscribe)
{
	return Internal::fromRxCpp(rxcpp::observable<>::create<var>([onSubscribe](rxcpp::subscriber<var> s) {
		onSubscribe(Subscriber([s](const var& next) {
			s.on_next(next);
		}));
	}));
}

Observable Observable::map(Transform1 transform)
{
	return Internal::fromRxCpp(internal->o.map(transform));
}

Observable Observable::combineLatest(Observable o1, Transform2& transform)
{
	return Internal::combineLatest(transform, *this, o1);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Transform3 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6, o7);
}

RXJUCE_NAMESPACE_END
