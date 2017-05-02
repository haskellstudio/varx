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

#include "rxjuce_Observer_Impl.h"

RXJUCE_SOURCE_PREFIX

namespace {
	// Wraps an Observable as a ReferenceCountedObject, so it can be stored in a juce var
	struct ReferenceCountedWrapper : public juce::ReferenceCountedObject
	{
		ReferenceCountedWrapper(const rxjuce::Observable& observable)
		: observable(observable) {}
		
		const rxjuce::Observable observable;
	};
	
	const std::string ObservableUnwrappingError("Error unwrapping Observable. This is likely because you called switchOnNext on an Observable that doesn't emit Observable items.");
}

namespace juce {
	// Converts an Observable to a JUCE var, and back
	template<>
	struct VariantConverter<rxjuce::Observable>
	{
		static rxjuce::Observable fromVar(const var &v)
		{
			ReferenceCountedObjectPtr<ReferenceCountedObject> ptr(v.getObject());
			
			if (auto wrapper = dynamic_cast<ReferenceCountedWrapper *>(ptr.get()))
				return wrapper->observable;
			else
				throw std::runtime_error(ObservableUnwrappingError);
		}
		
		static var toVar(const rxjuce::Observable& observable)
		{
			return new ReferenceCountedWrapper(observable);
		}
	};
}


RXJUCE_NAMESPACE_BEGIN

const std::function<void(std::exception_ptr)> Observable::TerminateOnError = [](std::exception_ptr) {
	// error implicitly ignored, abort
	std::terminate();
};

const std::function<void()> Observable::EmptyOnCompleted = [](){};

#pragma mark - Creation

Observable::Observable(const shared_ptr<Impl>& impl)
:	impl(impl) {}

Observable Observable::fromValue(Value value)
{
	return Impl::fromValue(value);
}

Observable Observable::just(const var& value)
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
		onSubscribe(Observer(std::make_shared<Observer::Impl>(s)));
	}));
}


#pragma mark - Subscription

Subscription Observable::subscribe(const std::function<void(const var&)>& onNext,
								   const std::function<void(std::exception_ptr)>& onError,
								   const std::function<void()>& onCompleted) const
{
	auto subscription = impl->wrapped.subscribe(onNext, onError, onCompleted);
	
	auto isSubscribed = [subscription]() { return subscription.is_subscribed(); };
	auto unsubscribe = [subscription]() { subscription.unsubscribe(); };
	
	return Subscription(isSubscribed, unsubscribe);
}

Subscription Observable::subscribe(const std::function<void(const var&)>& onNext,
								   const std::function<void()>& onCompleted,
								   const std::function<void(std::exception_ptr)>& onError) const
{
	return subscribe(onNext, onError, onCompleted);
}


#pragma mark - Operators

Observable Observable::map(Transform1 transform) const
{
	return Impl::fromRxCpp(impl->wrapped.map(transform));
}

Observable Observable::switchOnNext() const
{
	rxcpp::observable<rxcpp::observable<var>> unwrapped = impl->wrapped.map([](var observable) {
		return VariantConverter<Observable>::fromVar(observable).impl->wrapped;
	});
	
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


#pragma mark - Misc

Observable::operator var() const
{
	return VariantConverter<Observable>::toVar(*this);
}

RXJUCE_NAMESPACE_END
