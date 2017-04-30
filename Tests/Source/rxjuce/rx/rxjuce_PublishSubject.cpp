/*
  ==============================================================================

    rxjuce_PublishSubject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_PublishSubject.h"

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

RXJUCE_NAMESPACE_END
