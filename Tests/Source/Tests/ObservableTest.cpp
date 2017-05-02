/*
  ==============================================================================

    ObservableTest.cpp
    Created: 28 Apr 2017 8:11:40pm
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
		RxJUCECollectItems(Observable::range(Range<int>(3, 7), 3), items);
		RxJUCERequireItems(items, 3, 6, 7);
	}
	
	IT("emits double numbers with a double range") {
		RxJUCECollectItems(Observable::range(Range<double>(17.5, 22.8), 2), items);
		RxJUCERequireItems(items, 17.5, 19.5, 21.5, 22.8);
	}
	
	IT("emits just start if start == end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 10), 1), items);
		RxJUCERequireItems(items, 10);
	}
	
	IT("emits just start if start > end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 9), 1), items);
		RxJUCERequireItems(items, 10);
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


TEST_CASE("Observable::map",
		  "[Observable][Observable::map]")
{
	Array<var> items;
	auto source = Observable::range(Range<double>(4, 7), 2);
	
	IT("emits values synchronously") {
		auto mapped = source.map([](int i) { return i * 1.5; });
		RxJUCECollectItems(mapped, items);
		
		RxJUCERequireItems(items, 6.0, 9.0, 10.5);
	}
}


TEST_CASE("Interaction between Observable::map and Observable::switchOnNext",
		  "[Observable][Observable::map][Observable::switchOnNext]")
{
	Array<var> items;
	
	IT("supports returning Observables in map, even nested twice") {
		auto source = Observable::just(1);
		auto nested = source.map([](int i) {
			return Observable::just("Hello").map([i](String s) {
				return Observable::just(String(i) + " " + s);
			});
		});
		
		// Unwrap twice
		nested = nested.switchOnNext().switchOnNext();
		RxJUCECollectItems(nested, items);
		
		RxJUCERequireItems(items, "1 Hello");
	}
	
	IT("continues to emit items after the source Observable is gone") {
		auto source = std::make_shared<Observable>(Observable::just(17));
		auto mapped = source->map([](int next) {
			return Observable::create([next](Observer observer) {
				MessageManager::getInstance()->callAsync([observer, next]() mutable {
					observer.onNext(next * 3);
				});
			});
		});
		mapped = mapped.switchOnNext();
		RxJUCECollectItems(mapped, items);
		
		// There should be no items before running dispatch loop
		CHECK(items.isEmpty());
		
		source.reset();
		RxJUCERunDispatchLoop();
		
		// The item should be emitted, although there's no reference to the source anymore
		RxJUCERequireItems(items, 17 * 3);
	}
	
	IT("emits an error when trying to unwrap a first-order Observable") {
		auto o = Observable::just(1).switchOnNext();
		bool onErrorCalled = false;
		auto onError = [&](Error) {
			onErrorCalled = true;
		};
		auto subscription = o.subscribe([](var) {}, onError);
		
		REQUIRE(onErrorCalled);
	}
}


template<typename Var, typename... Vars>
var transform(Var v, Vars... vars)
{
	return v.toString() + transform(vars...).toString();
}

template<>
var transform<var>(var v)
{
	return v;
}

TEST_CASE("Observable::combineLatest",
		  "[Observable][Observable::combineLatest]")
{
	Array<var> items;
	OwnedArray<Observable> os;
	for (int i = 0; i < 8; i++)
		os.add(new Observable(Observable::just(String(i) + " ")));
	
	IT("works with arity 1") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], transform<var, var>), items);
		RxJUCERequireItems(items, "0 1 ");
	}
	
	IT("works with arity 2") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], transform<var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 ");
	}
	
	IT("works with arity 3") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], transform<var, var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 3 ");
	}
	
	IT("works with arity 4") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], transform<var, var, var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 3 4 ");
	}
	
	IT("works with arity 5") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], transform<var, var, var, var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 3 4 5 ");
	}
	
	IT("works with arity 6") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], *os[6], transform<var, var, var, var, var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 3 4 5 6 ");
	}
	
	IT("works with arity 7") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], *os[6], *os[7], transform<var, var, var, var, var, var, var, var>), items);
		RxJUCERequireItems(items, "0 1 2 3 4 5 6 7 ");
	}
}


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
