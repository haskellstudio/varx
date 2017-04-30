/*
  ==============================================================================

    rxjuce_PublishSubject.h
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observable.h"

RXJUCE_NAMESPACE_BEGIN

class PublishSubject
{
public:
	PublishSubject();
	
	void onNext(const juce::var& next);
	
	Observable getObservable() const;
	
private:
	class Internal;
	std::shared_ptr<Internal> internal;
	
	JUCE_LEAK_DETECTOR(PublishSubject)
};

RXJUCE_NAMESPACE_END
