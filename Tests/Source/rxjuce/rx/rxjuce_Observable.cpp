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

namespace {
	using namespace juce;
	
	struct RxCppObservableVarWrapper : public ReferenceCountedObject
	{
		RxCppObservableVarWrapper(const rxcpp::observable<var>& observable)
		: observable(observable) {}
		
		const rxcpp::observable<var> observable;
	};
	
	const std::string ObservableUnwrappingError("Error unwrapping Observable. This is likely because you called switchOnNext on an Observable that doesn't emit Observable items.");
}

namespace juce {
	template<>
	struct VariantConverter<rxcpp::observable<var>>
	{
		static rxcpp::observable<var> fromVar(const var &v)
		{
			if (!v.isObject())
				throw std::runtime_error(ObservableUnwrappingError);
			
			ReferenceCountedObjectPtr<ReferenceCountedObject> ptr(v.getObject());
			
			if (auto observableWrapper = dynamic_cast<RxCppObservableVarWrapper *>(ptr.get()))
				return observableWrapper->observable;
			
			throw std::runtime_error(ObservableUnwrappingError);
		}
		
		static var toVar(const rxcpp::observable<var>& observable)
		{
			return new RxCppObservableVarWrapper(observable);
		}
	};
}

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

Observable Observable::map(Transform1 transform) const
{
	return Impl::fromRxCpp(impl->wrapped.map(transform));
}

Observable Observable::map(const std::function<Observable(const var&)> transform) const
{
	rxcpp::observable<Observable> mapped = impl->wrapped.map(transform);
	
	return Impl::fromRxCpp(mapped.map([](const Observable& observable){
		return VariantConverter<rxcpp::observable<var>>::toVar(observable.impl->wrapped);
	}));
}

Observable Observable::flatMap(const std::function<Observable(const var&)>& inputCollectionSelector) const
{
	auto collectionSelector = [inputCollectionSelector](const var& value) {
		rxcpp::observable<var> observable = inputCollectionSelector(value).impl->wrapped;
		return observable;
	};
	
	auto flatMapped = impl->wrapped.flat_map(collectionSelector);
	
	return Impl::fromRxCpp(flatMapped);
}

Observable Observable::switchOnNext() const
{
	rxcpp::observable<rxcpp::observable<var>> unwrapped = impl->wrapped.map(VariantConverter<rxcpp::observable<var>>::fromVar);
	
	return Impl::fromRxCpp(unwrapped.switch_on_next());
}

Observable Observable::combineLatest(Observable o1, Transform2& transform) const
{
	return impl->combineLatest(transform, o1);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Transform3 transform) const
{
	return impl->combineLatest(transform, o1, o2);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform) const
{
	return impl->combineLatest(transform, o1, o2, o3);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform) const
{
	return impl->combineLatest(transform, o1, o2, o3, o4);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform) const
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform) const
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5, o6);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform) const
{
	return impl->combineLatest(transform, o1, o2, o3, o4, o5, o6, o7);
}

RXJUCE_NAMESPACE_END
