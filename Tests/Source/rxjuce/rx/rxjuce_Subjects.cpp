/*
  ==============================================================================

    rxjuce_Subject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subjects.h"

#include "rxjuce_Observable_Impl.h"
#include "rxjuce_Observer_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

struct BehaviorSubject::Impl
{
	Impl(const var& initial)
	: wrapped(initial) {}
	
	const rxcpp::subjects::behavior<var> wrapped;
};

BehaviorSubject::BehaviorSubject(const var& initial)
: impl(std::make_shared<Impl>(initial)) {}

void BehaviorSubject::onNext(const var& next)
{
	impl->wrapped.get_subscriber().on_next(next);
}

void BehaviorSubject::onError(Error error)
{
	impl->wrapped.get_subscriber().on_error(error);
}

void BehaviorSubject::onCompleted()
{
	impl->wrapped.get_subscriber().on_completed();
}

Observable BehaviorSubject::getObservable() const
{
	return Observable::Impl::fromRxCpp(impl->wrapped.get_observable());
}

Observer BehaviorSubject::getObserver()
{
	return std::make_shared<Observer::Impl>(impl->wrapped.get_subscriber());
}

var BehaviorSubject::getLatestItem() const
{
	return impl->wrapped.get_value();
}


struct PublishSubject::Impl
{
	const rxcpp::subjects::subject<var> wrapped;
};

PublishSubject::PublishSubject()
: impl(std::make_shared<Impl>()) {}

void PublishSubject::onNext(const var& next)
{
	impl->wrapped.get_subscriber().on_next(next);
}

void PublishSubject::onError(Error error)
{
	impl->wrapped.get_subscriber().on_error(error);
}

void PublishSubject::onCompleted()
{
	impl->wrapped.get_subscriber().on_completed();
}

Observable PublishSubject::getObservable() const
{
	return Observable::Impl::fromRxCpp(impl->wrapped.get_observable());
}

Observer PublishSubject::getObserver()
{
	return std::make_shared<Observer::Impl>(impl->wrapped.get_subscriber());
}

RXJUCE_NAMESPACE_END
