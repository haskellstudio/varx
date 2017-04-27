//
//  rxjuce_Subscription.h
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Prefixes.h"

RXJUCE_NAMESPACE_BEGIN

class Subscription
{
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

RXJUCE_NAMESPACE_END
