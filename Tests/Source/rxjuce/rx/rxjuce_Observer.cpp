/*
  ==============================================================================

    rxjuce_Observer.cpp
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observer.h"
#include "rxjuce_Observable.h"

#include "rxjuce_Observer_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

using std::placeholders::_1;

Observer::Observer(const std::shared_ptr<Impl>& impl)
: impl(impl) {}

void Observer::onNext(const juce::var& next) const
{
	impl->wrapped.on_next(next);
}

void Observer::onError(Error error) const
{
	impl->wrapped.on_error(error);
}

void Observer::onCompleted() const
{
	impl->wrapped.on_completed();
}

Subscription Observer::bindTo(const Observable& observable) const
{
	Observer self = *this;
	return observable.subscribe(std::bind(&Observer::onNext, self, std::placeholders::_1));
}

RXJUCE_NAMESPACE_END
