/*
  ==============================================================================

    rxjuce_Observer.cpp
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observer.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observer::Observer(const std::function<void(const juce::var&)>& onNext)
: _onNext(onNext)
{}

void Observer::onNext(const juce::var& next) const
{
	_onNext(next);
}

RXJUCE_NAMESPACE_END
