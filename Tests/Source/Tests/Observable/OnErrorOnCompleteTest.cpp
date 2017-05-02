/*
  ==============================================================================

    OnErrorOnCompleteTest.cpp
    Created: 2 May 2017 10:28:10pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"


TEST_CASE("Observable onError",
		  "[Observable][onError]")
{
	// Create an Observable that throws on subscribe
	auto syncThrow = Observable::create([](Observer){ throw std::runtime_error("Error!"); });
	
	IT("calls onError on subscribe") {
		REQUIRE_THROWS_WITH(syncThrow.subscribe([](var){}, std::rethrow_exception), "Error!");
	}
	
	IT("takes an onError handler and calls it without throwing") {
		bool called = false;
		syncThrow.subscribe([](var){}, [&](Error) {
			called = true;
		});
		
		REQUIRE(called);
	}
	
	IT("calls onError asynchronously") {
		// Create an Observable that throws asynchronously
		auto asyncThrow = Observable::create([](Observer observer) {
			MessageManager::getInstance()->callAsync([observer]() mutable {
				observer.onNext(3);
			});
		});
		asyncThrow = asyncThrow.map([](var v) {
			throw std::runtime_error("Async Error!");
			return v;
		});
		
		bool called = false;
		asyncThrow.subscribe([](var){}, [&](Error) {
			called = true;
		});
		
		CHECK_FALSE(called);
		RxJUCERunDispatchLoop();
		REQUIRE(called);
	}
}


TEST_CASE("Observable onComplete",
		  "[Observable][onComplete]")
{
	bool called = false;
	auto onComplete = [&]() {
		called = true;
	};
	
	IT("calls onComplete synchronously") {
		Observable::just(2).subscribe([](var){}, onComplete);
		REQUIRE(called);
	}
}
