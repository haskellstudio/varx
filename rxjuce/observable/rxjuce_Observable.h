//
//  rxjuce_Observable.h
//  RxJUCE-Tests
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Subscription.h"

#include <functional>

namespace rxjuce {
	class Observable
	{
	public:
		Subscription subscribe(const std::function<void(juce::var)>& f);
		
		static Observable just(juce::var value);
		
	private:
		struct Impl;
		typedef std::function<Impl()> GetImpl;
		
		Observable(const GetImpl& getImpl);
		
		const GetImpl getImpl;
	};
}
