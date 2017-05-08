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

/**
	A BehaviorSubject is a bridge between an Observer side (BehaviorSubject::onNext) and an Observable side (BehaviorSubject::getObservable). It is constructed with an initial item.
 
	When you can call BehaviorSubject::onNext with an item, the Observable side emits this item synchronously.
 
	On subscribe, it begins by emitting the most recently emitted item. It then continues to emit any items that are passed to onNext.
 
	The BehaviorSubject can be a model value of your app: Initialize it to some default value when the app starts, use the subject's Observable to update the corresponding GUI components, and call getValue when you need to serialize the app state.
 */
class BehaviorSubject
{
public:
	/** Creates a new instance with a given initial item */
	explicit BehaviorSubject(const juce::var& initial);
	
	/**
		Emits a new item. The Observable side emits this item synchronously.
	 */
	void onNext(const juce::var& next);
	
	/**
		Emits an error. The Observable side emits the error to all subscribers.
	 */
	void onError(Error error);
	
	/**
		Notifies that the subject has finished generating values. **It's illegal to call BehaviorSubject::onNext or BehaviorSubject::onError after calling this.**
	 */
	void onCompleted();
	
	/**
		Returns an Observable that emits an item whenever onNext is called on this subject.
	 
		On subscribe, the Observable emits the item most recently emitted by the BehaviorSubject (or the initial item). It then continues to emit any items that are passed to onNext.
	 */
	Observable getObservable() const;
	
	/**
		Returns the Observer side. If you call onNext on the Observer, this subject's Observable side will emit an item.
	 */
	Observer getObserver();
	
	/** Returns the most recently emitted item. If no items have been emitted, it returns the initial item. */
	juce::var getLatestItem() const;
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;

	JUCE_LEAK_DETECTOR(BehaviorSubject)
};

/**
	A subject that emits only those items that are passed to onNext *after the time of the subscription*.
 */
class PublishSubject
{
public:
	/** Creates a new instance. */
	PublishSubject();
	
	/**
		Emits a new item. The Observable side emits this item synchronously.
	 */
	void onNext(const juce::var& next);
	
	/**
		Emits an error. The Observable side emits the error to all subscribers.
	 */
	void onError(Error error);
	
	/**
		Notifies that the subject has finished generating values. **It's illegal to call PublishSubject::onNext or PublishSubject::onError after calling this.**
	 */
	void onCompleted();
	
	/**
		Returns an Observable that emits an item whenever onNext is called on this subject.
	 
		The Observable emits only those items that are passed to onNext *after the time of the subscription*.
	 */
	Observable getObservable() const;
	
	/**
		Returns the Observer side. If you call onNext on the Observer, this subject's Observable side will emit an item.
	 */
	Observer getObserver();
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;
	
	JUCE_LEAK_DETECTOR(PublishSubject)
};

RXJUCE_NAMESPACE_END
