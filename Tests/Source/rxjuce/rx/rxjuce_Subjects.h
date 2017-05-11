/*
  ==============================================================================

    rxjuce_Subject.h
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observer.h"
#include "rxjuce_Observable.h"

RXJUCE_NAMESPACE_BEGIN

/**
	A Subject is an Observer and an Observable at the same time. When Observer::onNext is called, the Observable emits an item.
 */
class Subject : public Observer, public Observable
{
public:
	/**
		Returns an Observable that emits an item whenever onNext is called on this subject.
	 */
	Observable getObservable() const;
	
	/**
		Returns the Observer side. If you call onNext on this Observer, this subject's Observable side will emit an item.
	 */
	Observer getObserver() const;
	
private:
	friend class BehaviorSubject;
	friend class PublishSubject;
	friend class BehaviorSubjectImpl;
	friend class PublishSubjectImpl;
	struct Impl;
	explicit Subject(const std::shared_ptr<Impl>& impl);
	std::shared_ptr<Impl> impl;
	
	JUCE_LEAK_DETECTOR(Subject)
};


/**
	A subject that starts with an initial item. On subscribe, it emits the most recently emitted item. It then continues to emit any items that are passed to onNext.
 */
class BehaviorSubject : public Subject
{
public:
	/** Creates a new instance with a given initial item */
	explicit BehaviorSubject(const juce::var& initial);
	
	/** Returns the most recently emitted item. If no items have been emitted, it returns the initial item. */
	juce::var getLatestItem() const;
	
private:
	JUCE_LEAK_DETECTOR(BehaviorSubject)
};


/**
	A subject that initially doesn't have a value. It does not emit an item on subscribe, and emits only those items that are passed to onNext *after the time of the subscription*.
 */
class PublishSubject : public Subject
{
public:
	/** Creates a new instance. */
	PublishSubject();
	
private:
	JUCE_LEAK_DETECTOR(PublishSubject)
};

RXJUCE_NAMESPACE_END
