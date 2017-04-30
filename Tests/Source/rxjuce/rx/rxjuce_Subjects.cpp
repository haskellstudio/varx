/*
  ==============================================================================

    rxjuce_Subject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subjects.h"

#include "rxjuce_Observable_Internal.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

class PublishSubject::Internal
{
public:
	Internal()
	: subject()
	{}
	
	rxcpp::subjects::subject<var> subject;
};

PublishSubject::PublishSubject()
: internal(std::make_shared<Internal>())
{}

void PublishSubject::onNext(const var& next)
{
	internal->subject.get_subscriber().on_next(next);
}

Observable PublishSubject::getObservable() const
{
	return Observable::Internal::fromRxCpp(internal->subject.get_observable());
}




class BehaviorSubject::Internal
{
public:
	Internal(const var& initial)
	: subject(initial)
	{}
	
	rxcpp::subjects::behavior<var> subject;
};

BehaviorSubject::BehaviorSubject(const juce::var& initial)
: internal(std::make_shared<Internal>(initial))
{}

void BehaviorSubject::onNext(const var& next)
{
	internal->subject.get_subscriber().on_next(next);
}

Observable BehaviorSubject::getObservable() const
{
	return Observable::Internal::fromRxCpp(internal->subject.get_observable());
}

juce::var BehaviorSubject::getValue() const
{
	return internal->subject.get_value();
}

RXJUCE_NAMESPACE_END
