//
//  rxjuce_Observable.h
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Subscription.h"

#include <functional>
#include <memory>
#include <cstddef>

class Observable
{
public:
	typedef juce::var var;
	
	//==============================================================================
	/**
	 Creation
	 */
	Observable(const juce::Value& value);
	
	static Observable just(var value);
	
	static Observable range(var first, var last, std::ptrdiff_t step);
	
	
	
	//==============================================================================
	/**
	 Subscription
	 */
	
	Subscription subscribe(const std::function<void(var)>& f);
	
	
	//==============================================================================
	/**
	 Transformation functions with different arity.
	 */
	
	typedef const std::function<var(const var&)>& Transform1;
	typedef const std::function<var(const var&, const var&)>& Transform2;
	typedef const std::function<var(const var&, const var&, const var&)>& Transform3;
	typedef const std::function<var(const var&, const var&, const var&, const var&)>& Transform4;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&)>& Transform5;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&)>& Transform6;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform7;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform8;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform9;
	typedef const std::function<var(const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&, const var&)>& Transform10;
	
	//==============================================================================
	/**
	 Operators
	 */
	
	Observable map(Transform1 transform);
	
	Observable combineLatest(Observable o1, Transform2 transform);
	Observable combineLatest(Observable o1, Observable o2, Transform3 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform);
	Observable combineLatest(const juce::Array<Observable>& others, const std::function<juce::var(const juce::Array<juce::var>&)>& transform);
	
#ifdef RXJUCE_ENABLE_LONG_SIGNATURES
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Observable o8, Transform9 transform);
	Observable combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Observable o8, Observable o9, Transform10 transform);
#endif
	
private:
	class Internal;
	Observable(const std::shared_ptr<Internal> & internal);
	std::shared_ptr<Internal>  internal;
};
