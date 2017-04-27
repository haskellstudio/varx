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
#include "rxjuce_LifetimeWatcher.h"
#include "rxjuce_LifetimeWatcherPool.h"

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observable Observable::fromValue(Value value)
{
	// Watches the lifetime of a ValueSource. Notifies a given subscriber whenever the ValueSource has set a new value.
	class ValueSourceListener : public LifetimeWatcher, private Value::Listener
	{
	public:
		ValueSourceListener(Value::ValueSource& source, Subscriber subscriber)
		:	source(&source),
			value(&source),
			subscriber(subscriber)
		{
			value.addListener(this);
		}
		
		const void* getAddress() const override
		{
			return source;
		}
		
		bool isExpired(int numLifetimeWatchers) const override
		{
			return (source->getReferenceCount() <= numLifetimeWatchers);
		}
		
	private:
		void valueChanged(Value &value) override
		{
			subscriber.onNext(value.getValue());
		}
		
		const Value::ValueSource* source;
		Value value;
		Subscriber subscriber;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueSourceListener)
	};
	
	return create([value](Subscriber subscriber) mutable {
		// Send the current value
		subscriber.onNext(value.getValue());
		
		// Add the listener to the pool, to keep getting updates from the value source
		LifetimeWatcherPool::getInstance().add(std::unique_ptr<LifetimeWatcher>(new ValueSourceListener(value.getValueSource(), subscriber)));
	});
}

Observable::Observable(const shared_ptr<Internal>& internal)
:	internal(internal)
{}

Subscription Observable::subscribe(const std::function<void(var)>& f) const
{
	const auto subscription = internal->o.subscribe(f);
	
	return Subscription([subscription]() {
		subscription.unsubscribe();
	});
}

Observable Observable::just(var value)
{
	return Internal::fromRxCpp(rxcpp::observable<>::just(value));
}

Observable Observable::range(var first, var last, int step)
{
	jassert(first.hasSameTypeAs(last));
	
	// Choose the rxcpp template parameter type depending on the var type
	std::function<Internal::Ptr(var, var, int)> createRange;
	if (first.isInt())
		createRange = Internal::range<int>;
	else if (first.isInt64())
		createRange = Internal::range<int64>;
	else if (first.isDouble())
		createRange = Internal::range<double>;
	else
		jassertfalse;
	
	return createRange(first, last, step);
}

Observable Observable::create(const std::function<void(const Subscriber&)>& onSubscribe)
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
