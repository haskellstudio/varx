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

class Subscriber;

class Observable
{
public:
	typedef juce::var var;
	
	//==============================================================================
	/**
	 Creation
	 */
	
	/**
		Creates an `Observable` from a given JUCE `Value`'s **source**.
	 
		The observable refers to the value's underlying `ValueSource` and continues to emit values even after the passed `Value` is destroyed.
	 
		**When calling setValue on a `Value`, the new value is emitted asynchronously.** If you call setValue immediately before destroying a Value, the new value will still be emitted.
	 */
	static Observable fromValue(juce::Value value);
	
	/**
		Creates an Observable which emits just a single value.
	 
		The value is emitted immediately on each new subscription.
	 */
	static Observable just(var value);
	
	/**
		Creates an Observable which emits a range between (and including) the first and last value, with a given step size.
	 
		For example:
	 
		```cpp
		range(3, 7, 3) -> {3, 6, 7}
		range(17.5, 22.8, 2) -> {17.5, 19.5, 21.5, 22.8}
		```
	 
		**The first and last values must have the same type! They must be int, int64 or double.**
	 */
	static Observable range(var first, var last, int step);
	
	/**
		Creates an Observable which emits values from a Subscriber on each subscription.
	 
		In your implementation of `onSubscribe`, you get a `Subscriber`. You can call `onNext` on it to emit values from the Observable.
	 */
	static Observable create(const std::function<void(Subscriber)>& onSubscribe);
	
	
	
	//==============================================================================
	/**
	 Subscription
	 */
	
	/**
		Subscribes to an Observable, to receive values it emits.
	 
		The onNext function is called whenever the Observable emits a new value.
	 
		The returned Subscription can be used to unsubscribe() from the Observable, to stop receiving values from it. **You will keep receiving values until you call unsubscribe**, so be careful.
	 */
	Subscription subscribe(const std::function<void(const var&)>& onNext) const;
	
	
	//==============================================================================
	/**
	 Operators
	 */
	
	// Transformation functions with different arity.
	typedef const std::function<var(const var&)>& Transform1;
	typedef const std::function<var(const var&, const var&)>& Transform2;
	typedef const std::function<var(const var&, const var&, const var&)>& Transform3;
	typedef const std::function<var(const var&, const var&, const var&, const var&)>& Transform4;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&)>& Transform5;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&)>& Transform6;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform7;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform8;
	
	/**
		Transforms the items emitted by this Observable by applying a given function to each emitted item.
	 */
	Observable map(Transform1 transform);
	
	/**
		When an item is emitted by either this Observable or o1, combines the latest item emitted by each Observable via the given function and emits the result of this function.
	 */
	Observable combineLatest(Observable o1, Transform2 transform);
	Observable combineLatest(Observable o1, Observable o2, Transform3 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform);
	
private:
	class Internal;
	Observable(const std::shared_ptr<Internal> & internal);
	std::shared_ptr<Internal>  internal;
	
	JUCE_LEAK_DETECTOR(Observable)
};

RXJUCE_NAMESPACE_END
