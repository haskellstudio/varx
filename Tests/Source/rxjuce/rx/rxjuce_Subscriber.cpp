/*
  ==============================================================================

    rxjuce_Subscriber.cpp
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subscriber.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Subscriber::Subscriber(const std::function<void(const juce::var&)>& onNext)
: _onNext(onNext)
{}

void Subscriber::onNext(const juce::var& next) const
{
	_onNext(next);
}

RXJUCE_NAMESPACE_END
