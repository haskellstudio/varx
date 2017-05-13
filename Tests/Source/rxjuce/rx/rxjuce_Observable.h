/*
  ==============================================================================

    rxjuce_Observable.h
    Created: 27 Apr 2017 7:07:33am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Disposable.h"

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
		Creates an Observable that immediately emits the items from the given Array.
	 
		Note that you can also pass an initializer list, like this:
	 
			Observable::from({"Hello", "Test"})
	 
			Observable::from({var(3), var("four")})
	 */
	static Observable from(const juce::Array<var>& array);
	
	/**
		Creates an Observable from a given JUCE Value. The returned Observable **only emits items until it is destroyed**, so you are responsible for managing its lifetime. Or use Reactive<Value>, which will handle this.
	 
		The returned Observable notifies the onComplete handler when it's destroyed. @see Observable::subscribe
	 
		When calling Value::setValue, it notifies asynchronously. So **the returned Observable emits the new value asynchronously.** If you call setValue immediately before destroying the returned Observable, the new value will not be emitted.
	 */
	static Observable fromValue(juce::Value value);
	
	/**
		Returns an Observable that emits one item every `interval`, starting at the time of disposable (where the first item is emitted). The emitted items are `1`, `2`, `3`, and so on.
	 
		The Observable emits endlessly, but you can use Observable::take to get a finite number of items (for example).
	 
		The interval has millisecond resolution.
	 */
	static Observable interval(const juce::RelativeTime& interval);
	
	/**
		Creates an Observable which emits a single element.
	 
		The value is emitted immediately on each new disposable.
	 */
	static Observable just(const var& value);
	
	///@{
	/**
		Creates an Observable which emits a range of items, starting at `first` to (and including) `last`. It completes after emitting the `last` item.
	 
		​ **Throws an exception if first > last.**
	 
		For example:
	 
			 Observable::range(3, 7, 3) // {3, 6, 7}
			 Observable::range(17.5, 22.8, 2) // {17.5, 19.5, 21.5, 22.8}
	 */
	static Observable range(int first, int last, unsigned int step = 1);
	/** \overload */
	static Observable range(double first, double last, unsigned int step);
	///@}
	
	/**
		Creates an Observable which emits values from an Observer on each disposable.
	 
		In the onSubscribe function, you get an Observer. You can call Observer::onNext on it to emit values from the Observable.
	 */
	static Observable create(const std::function<void(Observer)>& onSubscribe);
	
	
#pragma mark - Disposable
	///@{
	/**
		Subscribes to an Observable, to receive values it emits.
	 
		The **onNext** function is called whenever the Observable emits a new item. It may be called synchronously before subscribe() returns.
	 
		The **onError** function is called when the Observable has failed to generate the expected data, or has encountered some other error. If onError is called, the Observable will not make any more calls. **If you don't pass an onError handler, an exception inside the Observable will terminate your app.**
	 
		The **onCompleted** function is called exactly once to notify that the Observable has generated all data and will not emit any more items.
	 
		The returned Disposable can be used to unsubscribe from the Observable, to stop receiving values from it. **You will keep receiving values until you call Disposable::unsubscribe, or until the Observable source is destroyed**. You can use a ScopedDisposable, which automatically unsubscribes when it is destroyed.
	 */
	Disposable subscribe(const std::function<void(const var&)>& onNext,
						   const std::function<void(Error)>& onError = TerminateOnError,
						   const std::function<void()>& onCompleted = EmptyOnCompleted) const;
	/** \overload */
	Disposable subscribe(const std::function<void(const var&)>& onNext,
						   const std::function<void()>& onCompleted,
						   const std::function<void(Error)>& onError = TerminateOnError) const;
	///@}
	
	
#pragma mark - Transform Functions
	///@{
	/**
		A `FunctionN` is a function that takes `N` var arguments, and returns a var.
	 */
	typedef const std::function<var(const var&)>& Function1;
	typedef const std::function<var(const var&, const var&)>& Function2;
	typedef const std::function<var(const var&, const var&, const var&)>& Function3;
	typedef const std::function<var(const var&, const var&, const var&, const var&)>& Function4;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&)>& Function5;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&)>& Function6;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Function7;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Function8;
	///@}
	
#pragma mark - Operators
	///@{
	/**
		Returns an Observable that emits whenever an item is emitted by either this Observable or o1, o2, …. It combines the latest item emitted by each Observable via the given function and emits the result of this function.
	 */
	Observable combineLatest(Observable o1, Function2 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Function3 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Function4 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Function5 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Function6 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Function7 f) const;
	/** \overload */
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Function8 f) const;
	///@}

	///@{
	/**
		Returns an Observable that first emits the items from this Observable, then from o1, then from o2, and so on.
	 
		It only subscribes to o1 when this Observable has completed. And only subscribes to o2 when o1 has completed, and so on.
	 */
	Observable concat(Observable o1) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2, Observable o3) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2, Observable o3, Observable o4) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6) const;
	/** \overload */
	Observable concat(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7) const;
	///@}
	
	/**
		Returns an Observable which emits if `interval` has passed without this Observable emitting an item. The returned Observable emits the latest item from this Observable.
	 
		It's like the instant search in a search engine: Search suggestions are only loaded if the user hasn't pressed a key for a short period of time.
	 
		The interval has millisecond resolution.
	 */
	Observable debounce(const juce::RelativeTime& interval) const;
	
	Observable distinctUntilChanged() const;
	
	/**
		Returns an Observable that emits only those items from this Observable that pass a predicate function.
	 */
	Observable filter(const std::function<bool(const var&)>& predicate) const;
	
	/**
		For each emitted item, calls `f` and subscribes to the Observable returned from `f`. The emitted items from all these returned Observables are *merged* (so they interleave).
	 
		This Observable:
			
			Observable::from({"Hello", "World"}).flatMap([](String s) {
				return Observable::from({s.toLowerCase(), s.toUpperCase() + "!"});
			});
	 
		Will emit the items: `"hello"`, `"HELLO!"`, `"world"` and `"WORLD!"`.
	 
		@see Observable::merge, Observable::switchOnNext.
	 */
	Observable flatMap(const std::function<Observable(const var&)>& f) const;
	
	/**
		For each item emitted by this Observable, call the function with that item and emit the result.
	 
		If `f` returns an Observable, you can use Observable::switchOnNext afterwards.
	 */
	Observable map(Function1 f) const;
	
	///@{
	/**
		Merges the emitted items of this observable and o1, o2, … into one Observable. The items are interleaved, depending on when the source Observables emit items.

		An error in one of the source Observables notifies the result Observable's `onError` immediately.
	 */
	Observable merge(Observable o1) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2, Observable o3) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2, Observable o3, Observable o4) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6) const;
	/** \overload */
	Observable merge(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7) const;
	///@}
	
	/**
		Returns an Observable which checks every `interval` milliseconds whether this Observable has emitted any new items. If so, the returned Observable emits the latest item from this Observable.
	 
		For example, this is useful when an Observable emits items very rapidly, but you only want to update a GUI component 25 times per second to reduce CPU load.
	 
		The interval has millisecond resolution.
	 */
	Observable sample(const juce::RelativeTime& interval);
	
	/**
		Calls a function `f` with the given `startValue` and the first item emitted by this Observable. The value returned from `f` is remembered. When the second item is emitted, `f` is called with the remembered value (called the *accumulator*) and the second emitted item. The returned item is remembered, until the third item is emitted, and so on.
		
		The first parameter to `f` is the accumulator, the second is the current item.
	 */
	Observable scan(const var& startValue, Function2 f) const;
	
	/**
		Emits the given item(s) before beginning to emit the items in this Observable.
	 */
	Observable startWith(const var& item1) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3, const var& item4) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3, const var& item4, const var& item5) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3, const var& item4, const var& item5, const var& item6) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3, const var& item4, const var& item5, const var& item6, const var& item7) const;
	/** \overload */
	Observable startWith(const var& item1, const var& item2, const var& item3, const var& item4, const var& item5, const var& item6, const var& item7, const var& item8) const;
	
	/**
		​ **This must only be called if this Observable emits Observables**.
	 
		Returns an Observable that emits the items emitted by the Observables which this Observable emits.
	 */
	Observable switchOnNext() const;
	
	/**
		Returns an Observable that emits only the first `numItems` items from this Observable.
	 */
	Observable take(unsigned int numItems) const;
	
	Observable takeUntil(Observable other) const;
	
	
#pragma mark - Scheduling
	/**
		Returns an Observable that will be observed on a specified scheduler, for example the JUCE Message Thread or a background thread.
	 
		When you apply Observable::map to the returned Observable, the map function will run on the specified scheduler.
	 
		For example:
	 
			Observable::range(Range<double>(1, 1000), 1)
				.observeOn(Scheduler::newThread())
				.map([](double d){ return std::sqrt(d); }) // The lambda will be called on a new thread
				.observeOn(Scheduler::messageThread())
				.subscribe([&](double squareRoot) { }); // The lambda will be called on the message thread
	 
		@see Scheduler::messageThread, Scheduler::backgroundThread and Scheduler::newThread
	 */
	Observable observeOn(const Scheduler& scheduler) const;
	
	
#pragma mark - Misc
	/**
		Wraps the Observable into a var.
	 
		This allows you to return an Observable from a function, e.g. when using Observable::map.
	 */
	operator var() const;
	
	/**
		Blocks until the Observable has completed, then returns an Array of all emitted items.
	 
		Be careful when you use this on the message thread: If the Observable needs to process something *asynchronously* on the message thread, calling this will deadlock.
	 
		​ **If you don't pass an onError handler, an exception inside the Observable will terminate your app.**
	 */
	juce::Array<var> toArray(const std::function<void(Error)>& onError = TerminateOnError) const;
	
private:
	friend class Subject;
	struct Impl;
	Observable(const std::shared_ptr<Impl>&);
	std::shared_ptr<Impl> impl;
	
	static const std::function<void(Error)> TerminateOnError;
	static const std::function<void()> EmptyOnCompleted;
	JUCE_LEAK_DETECTOR(Observable)
};

RXJUCE_NAMESPACE_END
