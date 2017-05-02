/*
  ==============================================================================

    SubscriptionTest.cpp
    Created: 28 Apr 2017 8:12:29pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"
#include "rxjuce_Subscription.h"


TEST_CASE("Subscription",
		  "[Subscription]")
{
	// Create Observable which emits a single item asynchronously
	auto observable = std::make_shared<Observable>(Observable::create([](Observer observer) {
		MessageManager::getInstance()->callAsync([observer]() mutable {
			observer.onNext("Item");
		});
	}));
	
	// Subscribe to it
	Array<var> items;
	auto subscription = std::make_shared<Subscription>(observable->subscribe([&](String item) {
		items.add(item);
	}));
	
	IT("is initially subscribed") {
		REQUIRE(subscription->isSubscribed());
	}
	
	IT("is not subscribed after calling unsubscribe()") {
		subscription->unsubscribe();
		
		REQUIRE_FALSE(subscription->isSubscribed());
	}
	
	IT("received items while being subscribed") {
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Item");
	}
	
	IT("does not receive items after unsubscribing") {
		subscription->unsubscribe();
		RxJUCERunDispatchLoop();
		
		REQUIRE(items.isEmpty());
	}
	
	IT("takes ownership when move constructing") {
		Subscription other = std::move(*subscription);
		other.unsubscribe();
		RxJUCERunDispatchLoop();
		
		REQUIRE(items.isEmpty());
	}
	
	IT("does not unsubscribe when destroyed") {
		subscription.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Item");
	}
	
	IT("continues to receive items after the Observable is gone") {
		observable.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Item");
	}
	
	// Unsubscribe after each IT(), to prevent old subscriptions from filling the items array
	if (subscription)
		subscription->unsubscribe();
}


TEST_CASE("ScopedSubscription",
		  "[ScopedSubscription]")
{
	// Create Observable which emits a single item asynchronously
	auto observable = Observable::create([](Observer observer) {
		MessageManager::getInstance()->callAsync([observer]() mutable {
			observer.onNext("Item");
		});
	});
	
	// Subscribe to it
	Array<var> items;
	auto subscription = std::make_shared<ScopedSubscription>(observable.subscribe([&](String item) {
		items.add(item);
	}));
	
	IT("received items while not destroyed") {
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Item");
	}
	
	IT("does not receive items after being destroyed") {
		subscription.reset();
		RxJUCERunDispatchLoop();
		
		REQUIRE(items.isEmpty());
		
	}
}
