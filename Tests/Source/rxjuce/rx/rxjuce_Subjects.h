/*
  ==============================================================================

    rxjuce_Subject.h
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

class BehaviorSubject
{
public:
	BehaviorSubject(const juce::var& initial);
	
	void onNext(const juce::var& next);
	
	Observable getObservable() const;
	
	juce::var getValue() const;
	
private:
	class Internal;
	std::shared_ptr<Internal> internal;
	
	JUCE_LEAK_DETECTOR(BehaviorSubject)
};

RXJUCE_NAMESPACE_END
