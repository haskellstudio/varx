/*
  ==============================================================================

    rxjuce_Subject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subjects.h"

#include "rxjuce_Observable_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

struct BehaviorSubject::Impl
{
	Impl(const var& initial)
	: subject(initial) {}
	
	rxcpp::subjects::behavior<var> subject;
};

BehaviorSubject::BehaviorSubject(const var& initial)
: impl(new Impl(initial)) {}

BehaviorSubject::~BehaviorSubject() {}

void BehaviorSubject::onNext(const var& next)
{
	impl->subject.get_subscriber().on_next(next);
}

Observable BehaviorSubject::getObservable() const
{
	return Observable::Impl::fromRxCpp(impl->subject.get_observable());
}

var BehaviorSubject::getValue() const
{
	return impl->subject.get_value();
}


struct PublishSubject::Impl
{
	rxcpp::subjects::subject<var> subject;
};

PublishSubject::PublishSubject()
: impl(new Impl()) {}

PublishSubject::~PublishSubject() {}

void PublishSubject::onNext(const var& next)
{
	impl->subject.get_subscriber().on_next(next);
}

Observable PublishSubject::getObservable() const
{
	return Observable::Impl::fromRxCpp(impl->subject.get_observable());
}

RXJUCE_NAMESPACE_END
