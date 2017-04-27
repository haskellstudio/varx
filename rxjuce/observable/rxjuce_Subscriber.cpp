//
//  rxjuce_Subscriber.cpp
//  RxJUCE
//
//  Created by Martin Finke on 26.04.17.
//
//

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
