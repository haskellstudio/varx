/*
  ==============================================================================

    CreationTest.cpp
    Created: 2 May 2017 10:26:59pm
    Author:  Martin Finke

  ==============================================================================
 */

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"

using Catch::Contains;

TEST_CASE("Observable::just",
		  "[Observable][Observable::just]")
{
	Array<var> items;
	
	IT("emits a single value on subscribe") {
		RxJUCECollectItems(Observable::just(18.3), items);
		
		RxJUCERequireItems(items, 18.3);
	}
	
	IT("notifies multiple subscriptions") {
		Observable o = Observable::just("Hello");
		RxJUCECollectItems(o, items);
		RxJUCECollectItems(o, items);
		
		RxJUCERequireItems(items, "Hello", "Hello");
	}
}


TEST_CASE("Observable::range",
		  "[Observable][Observable::range]")
{
	Array<var> items;
	
	IT("emits integer numbers with an integer range") {
		RxJUCECollectItems(Observable::range(3, 7, 3), items);
		RxJUCERequireItems(items, 3, 6, 7);
	}
	
	IT("emits double numbers with a double range") {
		RxJUCECollectItems(Observable::range(17.5, 22.8, 2), items);
		RxJUCERequireItems(items, 17.5, 19.5, 21.5, 22.8);
	}
	
	IT("emits just start if start == end") {
		RxJUCECollectItems(Observable::range(10, 10), items);
		RxJUCERequireItems(items, 10);
	}
	
	IT("throws if start > end") {
		REQUIRE_THROWS_WITH(Observable::range(10, 9), Contains("Invalid range"));
	}
}


TEST_CASE("Observable::from",
		  "[Observable][Observable::from]")
{
	Array<var> items;
	
	IT("can be created from an Array<var>") {
		RxJUCECollectItems(Observable::from(Array<var>({3, 6, 8})), items);
		
		RxJUCERequireItems(items, 3, 6, 8);
	}
	
	IT("can be created from a std::initializer_list<var>") {
		RxJUCECollectItems(Observable::from({var("Hello"), var(15.5)}), items);
		
		RxJUCERequireItems(items, var("Hello"), var(15.5));
	}
	
	IT("can be created from a std::initializer_list<int>") {
		RxJUCECollectItems(Observable::from({1, 4}), items);
		
		RxJUCERequireItems(items, 1, 4);
	}
	
	IT("can be created from a std::initializer_list<String>") {
		RxJUCECollectItems(Observable::from({"Hello", "Test"}), items);
		
		RxJUCERequireItems(items, "Hello", "Test");
	}
	
}


TEST_CASE("Observable::fromValue",
		  "[Observable][Observable::fromValue]")
{
	Value value("Initial Item");
	const auto observable = Observable::fromValue(value);
	Array<var> items;
	RxJUCECollectItems(observable, items);
	
	RxJUCECheckItems(items, "Initial Item");
	
	IT("emits if a copy of the Value sets a new value") {
		Value copy(value);
		copy.setValue("Set by copy");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial Item", "Set by copy");
	}
	
	IT("emites only one item if the Value is set multiple times synchronously") {
		value = "2";
		value = "3";
		value = "4";
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial Item", "4");
	}
	
	IT("notifies multiple Subscriptions on subscribe") {
		Observable another = Observable::fromValue(value);
		RxJUCECollectItems(another, items);
		
		RxJUCERequireItems(items, "Initial Item", "Initial Item");
	}
	
	IT("notifies multiple Values referring to the same ValueSource") {
		Value anotherValue(value);
		Observable anotherObservable = Observable::fromValue(anotherValue);
		RxJUCECollectItems(anotherObservable, items);
		
		RxJUCERequireItems(items, "Initial Item", "Initial Item");
	}
	
	IT("notifies multiple Subscriptions if a Value is set multiple times") {
		ScopedSubscription another = observable.subscribe([&](String newValue) {
			items.add(newValue.toUpperCase());
		});
		
		value = "Bar";
		RxJUCERunDispatchLoop();
		
		value = "Baz";
		RxJUCERunDispatchLoop();
		
		CHECK(items.size() == 6);
		
		// Subscribers are notified in no particular order
		for (auto s : {"Initial Item", "INITIAL ITEM", "BAR", "Bar", "BAZ", "Baz"})
			REQUIRE(items.contains(s));
	}
}


TEST_CASE("Observable::fromValue lifetime",
		  "[Observable][Observable::fromValue]")
{
	// Create an Observable from a Value
	Value value("Initial");
	auto source = std::make_shared<Observable>(Observable::fromValue(value));
	
	// Create another Observable from the source Observable
	auto mapped = source->map([](String s){ return s; });
	
	// Collect items from the mapped Observable
	Array<var> items;
	RxJUCECollectItems(mapped, items);
	
	RxJUCECheckItems(items, "Initial");
	
	IT("emits items when the source Observable is alive") {
		value.setValue("New Value");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial", "New Value");
	}
	
	IT("stops emitting items as soon as the source Observable is destroyed") {
		source.reset();
		value.setValue("Two");
		value.setValue("Three");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial");
	}
	
	IT("does not emit an item if the Observable is destroyed immediately after calling setValue") {
		value.setValue("New Value");
		source.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, "Initial");
	}
	
	IT("continues to emit items if the source Observable is copied and then destroyed") {
		auto copy = std::make_shared<Observable>(*source);
		Array<var> copyItems;
		RxJUCECollectItems(*copy, copyItems);
		
		RxJUCECheckItems(copyItems, "Initial");
		
		source.reset();
		RxJUCERunDispatchLoop();
		value.setValue("New");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(copyItems, "Initial", "New");
	}
	
	IT("notified onComplete when the Observable is destroyed") {
		bool completed = false;
		source->subscribe([](var){}, [&]() { completed = true; });
		CHECK(!completed);
		
		source.reset();
		
		REQUIRE(completed);
	}
}


TEST_CASE("Observable::fromValue with a Slider",
		  "[Observable][Observable::fromValue]")
{
	Slider slider;
	slider.setValue(7.6);
	Observable o = Observable::fromValue(slider.getValueObject());
	Array<var> items;
	RxJUCECollectItems(o, items);
	RxJUCECheckItems(items, 7.6);
	
	IT("emits once if the Slider is changed once") {
		slider.setValue(0.45);
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, 7.6, 0.45);
	}
	
	IT("emits just once if the Slider value changes rapidly") {
		for (double value : {3.41, 9.54, 4.67, 3.56})
			slider.setValue(value);
		
		RxJUCERunDispatchLoop();
		
		RxJUCERequireItems(items, 7.6, 3.56);
	}
}


TEST_CASE("Observable::create",
		  "[Observable][Observable::create]")
{
	Array<var> items;
	
	IT("emits items when pushing items synchronously") {
		auto observable = Observable::create([](Observer observer) {
			observer.onNext("First");
			observer.onNext("Second");
		});
		RxJUCECollectItems(observable, items);
		
		RxJUCERequireItems(items, "First", "Second");
	}
	
	IT("emits items when pushing items asynchronously") {
		auto observable = Observable::create([](Observer observer) {
			MessageManager::getInstance()->callAsync([observer]() mutable {
				observer.onNext("First");
				observer.onNext("Second");
			});
		});
		RxJUCECollectItems(observable, items);
		
		// There shouldn't be any items until the async callback is executed
		CHECK(items.isEmpty());
		
		// The items should be there after running the dispatch loop
		RxJUCERunDispatchLoop();
		RxJUCERequireItems(items, "First", "Second");
	}
	
	IT("emits can emit items asynchronously after being destroyed") {
		auto observable = std::make_shared<Observable>(Observable::create([](Observer observer) {
			MessageManager::getInstance()->callAsync([observer]() mutable {
				observer.onNext("First");
				observer.onNext("Second");
			});
		}));
		
		IT("emits when there's still a subscription") {
			auto subscription = observable->subscribe([&](var next){ items.add(next); });
			observable.reset();
			RxJUCERunDispatchLoop();
			
			RxJUCERequireItems(items, "First", "Second");
		}
		
		IT("doesn't emit when the subscription has unsubscribed") {
			auto subscription = observable->subscribe([&](var next){ items.add(next); });
			observable.reset();
			subscription.unsubscribe();
			RxJUCERunDispatchLoop();
			
			REQUIRE(items.isEmpty());
		}
		
	}
	
	IT("calls onSubscribe again for each new subscription") {
		auto observable = Observable::create([](Observer observer) {
			observer.onNext("onSubscribe called");
		});
		RxJUCECollectItems(observable, items);
		RxJUCECollectItems(observable, items);
		RxJUCECollectItems(observable, items);
		
		RxJUCERequireItems(items, "onSubscribe called", "onSubscribe called", "onSubscribe called");
	}
	
	IT("captures an object until the Observable is destroyed") {
		// Create a ref counted object
		class Dummy : public ReferenceCountedObject
		{
		public:
			Dummy()
			: ReferenceCountedObject() {}
		};
		ReferenceCountedObjectPtr<ReferenceCountedObject> pointer(new Dummy());
		
		// Capture it in the Observable
		auto observable = std::make_shared<Observable>(Observable::create([pointer](Observer observer) {}));
		
		// There should be 2 references: From pointer and from the Observable
		CHECK(pointer->getReferenceCount() == 2);
		
		// If a copy of the Observable is made, it should still be 2
		auto copy = std::make_shared<Observable>(*observable);
		CHECK(pointer->getReferenceCount() == 2);
		
		// After the first Observable is destroyed, there should still be 2
		observable.reset();
		CHECK(pointer->getReferenceCount() == 2);
		
		// Creating a copy should not increase the ref count
		Subscription s = copy->subscribe([](var){});
		CHECK(pointer->getReferenceCount() == 2);
		
		// After the copy is destroyed, there should be just 1 (from the pointer)
		copy.reset();
		REQUIRE(pointer->getReferenceCount() == 1);
	}
}

TEST_CASE("Observable::interval",
		  "[Observable][Observable::interval]")
{
	IT("can create an interval below one second") {
		auto o = Observable::interval(RelativeTime::seconds(0.003)).take(3);
		auto lastTime = Time::getCurrentTime();
		Array<RelativeTime> intervals;
		Array<var> ints;
		o.subscribe([&](int i) {
			auto time = Time::getCurrentTime();
			intervals.add(time - lastTime);
			lastTime = time;
			ints.add(i);
		});
		
		CHECK(intervals.size() == 3);
		REQUIRE(intervals[0].inSeconds() == Approx(0).epsilon(0.01));
		REQUIRE(intervals[1].inSeconds() == Approx(0.003).epsilon(0.001));
		REQUIRE(intervals[2].inSeconds() == Approx(0.003).epsilon(0.001));
		
		RxJUCERequireItems(ints, 1, 2, 3);
	}
}
