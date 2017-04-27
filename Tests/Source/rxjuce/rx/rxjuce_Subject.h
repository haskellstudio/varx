/*
  ==============================================================================

    rxjuce_Subject.h
    Created: 27 Apr 2017 7:08:29am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefixes.h"
#include "rxjuce_Observable.h"

RXJUCE_NAMESPACE_BEGIN

class Subject
{
public:
	Subject();
	
	void onNext(const juce::var& next) const;
	
	Observable getObservable() const;
	
private:
	class Internal;
	std::shared_ptr<Internal> internal;
};

RXJUCE_NAMESPACE_END
