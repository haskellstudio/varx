/*
  ==============================================================================

    rxjuce_Observable.cpp
    Created: 27 Apr 2017 7:07:33am
    Author:  Martin Finke

  ==============================================================================
 */

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

#include "rxjuce_Observable.h"

#include "rxjuce_Observable_Impl.h"

#include "rxjuce_Observer.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observable Observable::fromValue(Value value)
{
	return Impl::fromValue(value);
}

Observable::Observable(const shared_ptr<Impl>& impl)
:	impl(impl)
{}

Subscription Observable::subscribe(const std::function<void(const var&)>& onNext) const
{
	auto subscription = impl->wrapped.subscribe(onNext);
	
	auto unsubscribe = [subscription]() { subscription.unsubscribe(); };
	auto isSubscribed = [subscription]() { return subscription.is_subscribed(); };

	return Subscription(isSubscribed, unsubscribe);
}

Observable Observable::just(var value)
{
	return Impl::fromRxCpp(rxcpp::observable<>::just(value));
}

Observable Observable::range(const juce::Range<int>& range, int step)
{
	auto o = rxcpp::observable<>::range<int>(range.getStart(), range.getEnd(), step);
	
	return Impl::fromRxCpp(o.map(juce::VariantConverter<int>::toVar));
}
Observable Observable::range(const juce::Range<double>& range, int step)
{
	auto o = rxcpp::observable<>::range<double>(range.getStart(), range.getEnd(), step);
	
	return Impl::fromRxCpp(o.map(juce::VariantConverter<double>::toVar));
}

Observable Observable::create(const std::function<void(Observer)>& onSubscribe)
{
	return Impl::fromRxCpp(rxcpp::observable<>::create<var>([onSubscribe](rxcpp::subscriber<var> s) {
		onSubscribe(Observer([s](const var& next) {
			s.on_next(next);
		}));
	}));
}

Observable Observable::map(Transform1 transform)
{
	return Impl::fromRxCpp(impl->wrapped.map(transform));
}

Observable Observable::combineLatest(Observable o1, Transform2& transform)
{
	return impl->combineLatest(transform, o1);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Transform3 transform)
{
	return impl->combineLatest(transform, o1, o2);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform)
{
	return impl->combineLatest(transform, o1, o2, o3);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform)
{
	return impl->combineLatest(transform, o1, o2, o3, o4);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform)
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform)
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5, o6);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform)
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5, o6, o7);
}

RXJUCE_NAMESPACE_END
