/*
  ==============================================================================

    ObserverTest.cpp
    Created: 2 May 2017 9:45:03pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"


TEST_CASE("Observer",
		  "[Observer]")
{
	IT("Emits items pushed to onNext") {
		auto o = Observable::create([](Observer observer) {
			observer.onNext(3);
			observer.onNext("Hello");
		});
		
		Array<var> items;
		RxJUCECollectItems(o, items);
		
		RxJUCERequireItems(items, var(3), var("Hello"));
	}
	
	IT("emits an error when calling onError") {
		auto o = Observable::create([](Observer observer) {
			observer.onError(Error());
		});
		
		bool onErrorCalled = false;
		o.subscribe([](var){}, [&](Error) { onErrorCalled = true; });
		REQUIRE(onErrorCalled);
	}
	
	IT("notifies onCompleted") {
		auto o = Observable::create([](Observer observer) {
			observer.onCompleted();
		});
		
		bool completed = false;
		o.subscribe([](var){}, [&](){ completed = true; });
		
		REQUIRE(completed);
	}
	
	IT("can be bound to an Observable") {
#warning TODO Test Observer::bindTo
	}
}
