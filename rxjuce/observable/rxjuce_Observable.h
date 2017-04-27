//
//  rxjuce_Observable.h
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Subscription.h"

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
		Creates a new `Observable` from a given JUCE `Value`'s **source**. The observable refers to the value's underlying `ValueSource` and continues to emit values even after the `Value` disappears.
	 */
	static Observable fromValue(juce::Value value);
	
	static Observable just(var value);
	
	static Observable range(var first, var last, int step);
	
	static Observable create(const std::function<void(const Subscriber&)>& onSubscribe);
	
	
	
	//==============================================================================
	/**
	 Subscription
	 */
	
	Subscription subscribe(const std::function<void(var)>& f) const;
	
	
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
	
	
	Observable map(Transform1 transform);
	
	Observable combineLatest(Observable o1, Transform2 transform);
	Observable combineLatest(Observable o1, Observable o2, Transform3 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform);
	
private:
	class Internal;
	friend class Subject;
	Observable(const std::shared_ptr<Internal> & internal);
	std::shared_ptr<Internal>  internal;
};
