/*
  ==============================================================================

    LifetimeWatcherPoolTest.cpp
    Created: 28 Apr 2017 8:11:53pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"
#include "rxjuce_LifetimeWatcherPoolFixture.h"


TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "Values in LifetimeWatcherPool",
				 "[LifetimeWatcherPool]")
{
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

TEST_CASE_METHOD(LifetimeWatcherPoolFixture,
				 "WeakReferenceLifetimeWatcher in LifetimeWatcherPool",
				 "[LifetimeWatcherPool]")
{	
	// A watched object that sends a String when it's deleted
	class WatchedObject
	{
	public:
		WatchedObject(std::function<void(String)> notify)
		: notify(notify)
		{}
		
		~WatchedObject()
		{
			notify(String("~WatchedObject"));
			masterReference.clear();
		}
		
	private:
		WeakReference<WatchedObject>::Master masterReference;
		friend class WeakReference<WatchedObject>;
		
		const std::function<void(String)> notify;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WatchedObject)
	};
	
	// A lifetime watcher for the above object, which also sends a String when it's deleted
	class Watcher : public WeakReferenceLifetimeWatcher<WatchedObject>
	{
	public:
		Watcher(const juce::WeakReference<WatchedObject>& ref, std::function<void(String)> notify)
		: WeakReferenceLifetimeWatcher<WatchedObject>(ref),
		  notify(notify)
		{}
		
		~Watcher()
		{
			notify(String("~Watcher"));
		}
		
	private:
		const std::function<void(String)> notify;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Watcher)
	};
	
	Array<var> log;
	
	// Create watched object
	ScopedPointer<WatchedObject> watchedObject(new WatchedObject([&](String message) {
		log.add(message);
	}));
	
	// Create watcher and add it to pool
	pool.add(std::make_unique<Watcher>(juce::WeakReference<WatchedObject>(watchedObject), [&](String message) {
		log.add(message);
	}));
	
	REQUIRE(log.isEmpty());
	
	// Delete watched object
	watchedObject = nullptr;
	
	RxJUCERequireResults(log, "~WatchedObject");
	
	// Watcher should be there, until removeExpiredWatchers is called
	REQUIRE(pool.getNumWatchers() == 1);
	pool.removeExpiredWatchers();
	REQUIRE(pool.getNumWatchers() == 0);
	
	RxJUCERequireResults(log, "~WatchedObject", "~Watcher");
}
