/*
  ==============================================================================

    rxjuce_Observable.h
    Created: 27 Apr 2017 7:07:33am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Subscription.h"

RXJUCE_NAMESPACE_BEGIN

class Observer;

/**
	An Observable is a value that changes over time.
 */
class Observable
{
public:
	//! @cond Doxygen_Suppress
	typedef juce::var var;
	//! @endcond
	
#pragma mark - Creation
	/**
		Creates an Observable from a given JUCE Value. The returned Observable **only emits items until it is destroyed**, so you are responsible for managing its lifetime. Or use Observed<Value>, which will handle this.
	 
		When calling Value::setValue, it notifies asynchronously. So **the returned Observable will emits the new value asynchronously.** So if you call setValue immediately before destroying the returned Observable, the new value will not be emitted.
	 */
	static Observable fromValue(juce::Value value);
	
	/**
		Creates an Observable which emits just a single item.
	 
		The value is emitted immediately on each new subscription.
	 */
	static Observable just(var value);
	
	/**	@name range
		
	 */
	///@{
	/**
		Creates an Observable which emits values from a given range, starting at range.getStart() to (and including) range.getEnd().
	 
		For example:
	 
			 Observable::range(Range<int>(3, 7), 3) // {3, 6, 7}
			 Observable::range(Range<double>(17.5, 22.8), 2) // {17.5, 19.5, 21.5, 22.8}
	 */
	static Observable range(const juce::Range<int>& range, int step);
	static Observable range(const juce::Range<double>& range, int step);
	///@}
	
	/**
		Creates an Observable which emits values from a Subscriber on each subscription.
	 
		In your implementation of onSubscribe, you get an Observer. You can call Observer::onNext on it to emit values from the Observable.
	 */
	static Observable create(const std::function<void(Observer)>& onSubscribe);
	
	
#pragma mark - Subscription
	/**
		Subscribes to an Observable, to receive values it emits.
	 
		The onNext function is called whenever the Observable emits a new item. It may be called synchronously before subscribe() returns.
	 
		The returned Subscription can be used to unsubscribe() from the Observable, to stop receiving values from it. **You will keep receiving values until you call Subscription::unsubscribe, or until the Observable source is destroyed**. You can use a RAIISubscription, which automatically unsubscribes when it is destroyed.
	 */
	Subscription subscribe(const std::function<void(const var&)>& onNext) const;
	
	
#pragma mark - Transform Functions
	///@{
	/**
		A function which takes one or more vars as input, and outputs a var.
	 */
	typedef const std::function<var(const var&)>& Transform1;
	typedef const std::function<var(const var&, const var&)>& Transform2;
	typedef const std::function<var(const var&, const var&, const var&)>& Transform3;
	typedef const std::function<var(const var&, const var&, const var&, const var&)>& Transform4;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&)>& Transform5;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&)>& Transform6;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform7;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform8;
	///@}
	
#pragma mark - Operators
	/**
		Transforms the items emitted by this Observable by applying a given function to each emitted item.
	 
		The transform function can return `Observable``s`. If it does, you can use Observable::switchOnNext.
	 */
	Observable map(Transform1 transform) const;
	
	/**
		This **must only be called if this Observable emits Observables**.
	 
		Returns an Observable that emits the items emitted by the Observables which this Observable emits.
	 */
	Observable switchOnNext() const;
	
	///@{
	/**
		When an item is emitted by either this Observable or o1, o2, …, combines the latest item emitted by each Observable via the given function and emits the result of this function.
	 */
	Observable combineLatest(Observable o1, Transform2 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Transform3 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform) const;
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform) const;
	///@}
	
#pragma mark - Misc
	/**
		Wraps the Observable into a var. This allows you to return an Observable from a transform function.
	 */
	operator var() const;
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;
	
	friend class BehaviorSubject;
	friend class PublishSubject;
	Observable(const std::shared_ptr<Impl>&);
	
	JUCE_LEAK_DETECTOR(Observable)
};

RXJUCE_NAMESPACE_END
