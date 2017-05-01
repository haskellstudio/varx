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
}
