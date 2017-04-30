/*
  ==============================================================================

    rxjuce_BehaviorSubject.cpp
    Created: 30 Apr 2017 1:59:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_BehaviorSubject.h"

#include "rxjuce_Observable_Internal.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

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
