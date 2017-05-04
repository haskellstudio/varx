/*
  ==============================================================================

    SchedulingTest.cpp
    Created: 3 May 2017 8:26:03am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"
#include "rxjuce_Scheduler.h"


TEST_CASE("Observable::observeOn",
		  "[Observable][Observable::observeOn]")
{
	auto observable = Observable::from({1, 2, 3});
	Array<var> items;
	
	IT("can schedule to a background thread and two new threads") {
		CHECK(MessageManager::getInstance()->isThisTheMessageThread());
		auto messageThreadID = Thread::getCurrentThreadId();
		CHECK(messageThreadID != nullptr);
		
		Thread::ThreadID backgroundThreadID = nullptr;
		Thread::ThreadID newThreadID = nullptr;
		Thread::ThreadID anotherNewThreadID = nullptr;
		
		// Observe on background thread
		auto onBackgroundThread = observable.observeOn(Scheduler::backgroundThread()).map([&](int i) {
			backgroundThreadID = Thread::getCurrentThreadId();
			return i * 2;
		});
		
		// Observe on new thread
		auto onNewThread = onBackgroundThread.observeOn(Scheduler::newThread()).map([&](int i) {
			newThreadID = Thread::getCurrentThreadId();
			return i * 3;
		});
		
		auto onAnotherNewThread = onNewThread.observeOn(Scheduler::newThread()).map([&](int i) {
			anotherNewThreadID = Thread::getCurrentThreadId();
			return i * 4;
		});
		
		// Wait blocking
		items = onAnotherNewThread.toArray();
		
		// Check that no thread IDs are equal, and all are != nullptr
		SortedSet<Thread::ThreadID> threadIDs;
		threadIDs.add(messageThreadID);
		threadIDs.add(backgroundThreadID);
		threadIDs.add(newThreadID);
		threadIDs.add(anotherNewThreadID);
		threadIDs.add(nullptr);
		
		REQUIRE(threadIDs.size() == 5);
		RxJUCERequireItems(items, 24, 48, 72);
	}
	
	IT("can schedule to the message thread") {
		auto onMessageThread = observable.observeOn(Scheduler::messageThread()).map([](int i) {
			return i * 2;
		});
		RxJUCECollectItems(onMessageThread, items);
		
		// There shouldn't be any items yet, because observeOn is asynchronous
		CHECK(items.isEmpty());
		
		// The message thread scheduler only runs with 60 Hz, so we have to wait about 16 ms
		RxJUCERunDispatchLoop(20);
		
		RxJUCERequireItems(items, 2, 4, 6);
	}
}
