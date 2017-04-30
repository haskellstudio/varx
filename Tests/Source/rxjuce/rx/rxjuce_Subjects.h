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
	~PublishSubject();
	
	void onNext(const juce::var& next);
	
	Observable getObservable() const;
	
private:
	class Impl;
	const juce::ScopedPointer<Impl> impl;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PublishSubject)
};

class BehaviorSubject
{
public:
	BehaviorSubject(const juce::var& initial);
	~BehaviorSubject();
	
	void onNext(const juce::var& next);
	
	Observable getObservable() const;
	
	juce::var getValue() const;
	
private:
	class Impl;
	const juce::ScopedPointer<Impl> impl;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BehaviorSubject)
};

RXJUCE_NAMESPACE_END
