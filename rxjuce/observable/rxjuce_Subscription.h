//
//  rxjuce_Subscription.h
//  RxJUCE-Tests
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "JuceHeader.h"

#include <functional>

namespace rxjuce {
	class Subscription {
	public:
		Subscription(Subscription&& other);
		Subscription& operator=(Subscription&& other);
		~Subscription();
		
		void unsubscribe();
		
	private:
		friend class Observable;
		Subscription(const std::function<void()>& unsubscribe);
		
		std::function<void()> _unsubscribe;
	};
}
