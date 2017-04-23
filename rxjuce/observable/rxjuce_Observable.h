//
//  rxjuce_Observable.h
//  RxJUCE-Tests
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Subscription.h"

#include "JuceHeader.h"

#include <functional>

namespace rxjuce {
	class Observable {
	public:
		Subscription subscribe(const std::function<void(var)>& f);
		
		static Observable just(var value);
		
	private:
		struct Impl;
		typedef std::function<Impl()> GetImpl;
		
		Observable(const GetImpl& getImpl);
		
		const GetImpl getImpl;
	};
}
