//
//  rxjuce_Subject.cpp
//  RxJUCE
//
//  Created by Martin Finke on 27.04.17.
//
//

#include "rxjuce_Subject.h"

#include "rxjuce_Observable_Internal.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

class Subject::Internal
{
public:
	rxcpp::subjects::subject<var> s;
};

Subject::Subject()
: internal(std::make_shared<Internal>())
{}

void Subject::onNext(const juce::var& next) const
{
	internal->s.get_subscriber().on_next(next);
}

Observable Subject::getObservable() const
{
	return Observable::Internal::fromRxCpp(internal->s.get_observable());
}

RXJUCE_NAMESPACE_END
