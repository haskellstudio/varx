/*
  ==============================================================================

    LifetimeWatcherPoolTest.cpp
    Created: 28 Apr 2017 8:11:53pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

TEST_CASE("LifetimeWatcherPool",
		  "[LifetimeWatcherPool]")
{
	auto& pool = LifetimeWatcherPool::getInstance();
	pool.removeExpiredWatchers();
	
	IT("initially has 0 watchers") {
		REQUIRE(pool.getNumWatchers() == 0);
	}
	
	CONTEXT("A watcher is added") {
		auto value = std::make_shared<Value>();
		RAIISubscription s = Observable::fromValue(*value).subscribe([](var){});
		
		IT("has 1 watcher") {
			REQUIRE(pool.getNumWatchers() == 1);
		}
		
		IT("removes the watcher after destroying the value and calling removeExpiredWatchers") {
			value.reset();
			REQUIRE(pool.getNumWatchers() == 1);
			pool.removeExpiredWatchers();
			REQUIRE(pool.getNumWatchers() == 0);
		}
		
		CONTEXT("ValueSource ref count") {
			Value::ValueSource* source = &value->getValueSource();
			
			IT("is initially 2: 1 from Value + 1 from LifetimeWatcher") {
				REQUIRE(source->getReferenceCount() == 2);
			}
			
			IT("becomes 1 when the Value is destroyed") {
				value.reset();
				CHECK(pool.getNumWatchers() == 1);
				REQUIRE(source->getReferenceCount() == 1);
			}
			
			IT("is not 1 after calling removeExpiredWatchers") {
				value.reset();
				pool.removeExpiredWatchers();
				CHECK(pool.getNumWatchers() == 0);
				
				// This is technically undefined behavior, but there's no other way to check whether the ref count has really gone to 0.
				REQUIRE(source->getReferenceCount() != 1);
			}
		}
	}
}
