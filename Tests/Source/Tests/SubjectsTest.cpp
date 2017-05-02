/*
  ==============================================================================

    SubjectsTest.cpp
    Created: 30 Apr 2017 11:25:48am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Subjects.h"


TEST_CASE("BehaviorSubject",
		  "[BehaviorSubject]")
{
	BehaviorSubject subject("Initial Item");
	
	// Subscribe to the subject's Observable
	Array<var> items;
	RxJUCECollectItems(subject.getObservable(), items);
	
	IT("has the initial item after being created") {
		CHECK(subject.getValue() == "Initial Item");
		RxJUCERequireItems(items, "Initial Item");
	}
	
	IT("emits when pushing a new item") {
		RxJUCECheckItems(items, "Initial Item");
		
		subject.onNext("New Item");
		
		CHECK(subject.getValue() == "New Item");
		RxJUCERequireItems(items, "Initial Item", "New Item");
	}
	
	IT("emits an error when calling onError") {
		BehaviorSubject subject(17);
		bool onErrorCalled = false;
		subject.onError(Error());
		subject.getObservable().subscribe([](var){}, [&](Error) { onErrorCalled = true; });
		REQUIRE(onErrorCalled);
	}
	
	IT("notifies onCompleted when calling onCompleted") {
		bool completed = false;
		subject.getObservable().subscribe([](var){}, [&](){ completed = true; });
		subject.onCompleted();
		
		REQUIRE(completed);
	}
	
	IT("calls onCompleted when destroying the subject") {
		auto subject = std::make_shared<BehaviorSubject>(3);
		bool completed = false;
		subject->getObservable().subscribe([](var){}, [&](){ completed = true; });
		subject.reset();
		
		REQUIRE(completed);
	}
}


TEST_CASE("PublishSubject",
		  "[PublishSubject]")
{
	PublishSubject subject;
	
	// Subscribe to the subject's Observable
	Array<var> items;
	RxJUCECollectItems(subject.getObservable(), items);
	
	IT("does not emit an item if nothing has been pushed") {
		REQUIRE(items.isEmpty());
	}
	
	IT("emits when pushing a new item") {
		CHECK(items.isEmpty());
		
		subject.onNext("First Item");
	
		RxJUCERequireItems(items, "First Item");
	}
	
	IT("does not emit previous item(s) when subscribing") {
		subject.onNext(1);
		subject.onNext(2);
		RxJUCECheckItems(items, 1, 2);
		
		Array<var> laterItems;
		RxJUCECollectItems(subject.getObservable(), laterItems);
		
		REQUIRE(laterItems.isEmpty());
	}
	
	IT("emits an error when calling onError") {
		PublishSubject subject;
		bool onErrorCalled = false;
		subject.onError(Error());
		subject.getObservable().subscribe([](var){}, [&](Error) { onErrorCalled = true; });
		REQUIRE(onErrorCalled);
	}
	
	CONTEXT("onCompleted") {
		auto subject = std::make_shared<PublishSubject>();
		bool completed = false;
		
		IT("notifies onCompleted when calling onCompleted") {
			subject->onCompleted();
			subject->getObservable().subscribe([](var){}, [&](){ completed = true; });
			
			REQUIRE(completed);
		}
		
		IT("calls onCompleted when destroying the subject") {
			subject->getObservable().subscribe([](var){}, [&](){ completed = true; });
			CHECK(!completed);
			subject.reset();
			
			REQUIRE(completed);
		}
		
		IT("calls onCompleted when the subject is already destroyed") {
			auto o = subject->getObservable();
			subject.reset();
			o.subscribe([](var){}, [&](){ completed = true; });
			
			REQUIRE(completed);
		}
	}
	
}
