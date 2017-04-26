//
//  rxjuce_Subscription.cpp
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#include "rxjuce_Subscription.h"

#include <utility>

Subscription::Subscription(const std::function<void()>& unsubscribe)
: _unsubscribe(unsubscribe)
{}

Subscription::Subscription(Subscription&& other)
{
	std::swap(_unsubscribe, other._unsubscribe);
}

Subscription& Subscription::operator=(Subscription&& other)
{
	std::swap(_unsubscribe, other._unsubscribe);
	return *this;
}

void Subscription::unsubscribe()
{
	_unsubscribe();
}

Subscription::~Subscription()
{
	unsubscribe();
}
