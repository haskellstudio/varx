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
class Scheduler;

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
		Creates an Observable that immediately emits the items from the given Array to each subscriber.
	 
		Note that you can also pass a std::initializer_list here, like this:
	 
			Observable::from({"Hello", "Test"})
	 
			Observable::from({var(3), var("four")})
	 */
	static Observable from(const juce::Array<var>& array);
	
	/**
		Creates an Observable from a given JUCE Value. The returned Observable **only emits items until it is destroyed**, so you are responsible for managing its lifetime. Or use Observed<Value>, which will handle this.
	 
		The returned Observable notifies the onComplete handler when it's destroyed. @see Observable::subscribe
	 
		When calling Value::setValue, it notifies asynchronously. So **the returned Observable emits the new value asynchronously.** If you call setValue immediately before destroying the returned Observable, the new value will not be emitted.
	 */
	static Observable fromValue(juce::Value value);
	
	/**
		Returns an Observable that emits one item every `period`, starting at the time of subscription (where the first item is emitted). The emitted items are `1`, `2`, `3`, and so on. The interval has millisecond resolution.
	 
		The Observable emits endlessly, but you can use Observable::take to get a finite number of items (for example).
	 */
	static Observable interval(const juce::RelativeTime& period);
	
	/**
		Creates an Observable which emits just a single item.
	 
		The value is emitted immediately on each new subscription.
	 */
	static Observable just(const var& value);
	
	///@{
	/**
		Creates an Observable which emits values from a given range, starting at `first` to (and including) `last`.
	 
		​ **Throws an exception if first > last.**
	 
		For example:
	 
			 Observable::range(3, 7, 3) // {3, 6, 7}
			 Observable::range(17.5, 22.8, 2) // {17.5, 19.5, 21.5, 22.8}
	 */
	static Observable range(int first, int last, unsigned int step);
	static Observable range(double first, double last, unsigned int step);
	///@}
	
	/**
		Creates an Observable which emits values from a Subscriber on each subscription.
	 
		In your implementation of onSubscribe, you get an Observer. You can call Observer::onNext on it to emit values from the Observable.
	 */
	static Observable create(const std::function<void(Observer)>& onSubscribe);
	
	
#pragma mark - Subscription
	///@{
	/**
		Subscribes to an Observable, to receive values it emits.
	 
		The **onNext** function is called whenever the Observable emits a new item. It may be called synchronously before subscribe() returns.
	 
		The **onError** function is called when the Observable has failed to generate the expected data, or has encountered some other error. If onError is called, the Observable will not make any more calls. **If you don't pass an onError handler, an exception inside the Observable will terminate your app.**
	 
		The **onCompleted** function is called exactly once to notify that the Observable has generated all data and will not emit any more items.
	 
		The returned Subscription can be used to unsubscribe from the Observable, to stop receiving values from it. **You will keep receiving values until you call Subscription::unsubscribe, or until the Observable source is destroyed**. You can use a ScopedSubscription, which automatically unsubscribes when it is destroyed.
	 */
	Subscription subscribe(const std::function<void(const var&)>& onNext,
						   const std::function<void(Error)>& onError = TerminateOnError,
						   const std::function<void()>& onCompleted = EmptyOnCompleted) const;
	
	Subscription subscribe(const std::function<void(const var&)>& onNext,
						   const std::function<void()>& onCompleted,
						   const std::function<void(Error)>& onError = TerminateOnError) const;
	///@}
	
	
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
	
	/**
		Returns an Observable which emits only those items from this Observable that pass a predicate function.
	 */
	Observable filter(const std::function<bool(const var&)>& predicate) const;
	
	/**
		For each item emitted by this Observable, call the transform function with that item and emit the result.
	 
		You can return an Observable from the transform function. If you do, you can use Observable::switchOnNext.
	 */
	Observable map(Transform1 transform) const;
	
	/**
		​ **This must only be called if this Observable emits Observables**.
	 
		Returns an Observable that emits the items emitted by the Observables which this Observable emits.
	 */
	Observable switchOnNext() const;
	
	/**
		Returns an Observable that emits only the first `numItems` items from this Observable.
	 */
	Observable take(unsigned int numItems) const;
	
	
#pragma mark - Scheduling
	/**
		Returns an Observable that will be observed on a specified scheduler, for example the JUCE Message Thread or a background thread.
	 
		When you apply Observable::map to the returned Observable, the map transform function will run on the specified scheduler.
	 
		For example:
	 
			Observable::range(Range<double>(1, 1000), 1)
				.observeOn(Scheduler::newThread())
				.map([](double d){ return std::sqrt(d); }) // Will run on a new thread
				.observeOn(Scheduler::messageThread())
				.subscribe([&](double squareRoot) { }); // Will be called on the message thread
	 
		@see Scheduler::messageThread, Scheduler::backgroundThread and Scheduler::newThread
	 */
	Observable observeOn(const Scheduler& scheduler) const;
	
	
#pragma mark - Misc
	/**
		Wraps the Observable into a var.
	 
		This allows you to return an Observable from a transform function, e.g. when using Observable::map.
	 */
	operator var() const;
	
	/**
		Blocks until the Observable has completed, then returns an Array of all emitted items.
	 
		Be careful when you use this on the message thread: If the Observable needs to process something *asynchronously* on the message thread, calling this will deadlock.
	 
		​ **If you don't pass an onError handler, an exception inside the Observable will terminate your app.**
	 */
	juce::Array<var> toArray(const std::function<void(Error)>& onError = TerminateOnError) const;
	
private:
	struct Impl;
	std::shared_ptr<Impl> impl;
	
	friend class BehaviorSubject;
	friend class PublishSubject;
	Observable(const std::shared_ptr<Impl>&);
	
	static const std::function<void(Error)> TerminateOnError;
	static const std::function<void()> EmptyOnCompleted;
	JUCE_LEAK_DETECTOR(Observable)
};

RXJUCE_NAMESPACE_END
