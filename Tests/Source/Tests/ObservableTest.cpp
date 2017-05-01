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
	Array<var> results;
	
	IT("emits a single value on subscribe") {
		RxJUCECollectItems(Observable::just(18.3), results);
		
		RxJUCERequireResults(results, 18.3);
	}
	
	IT("notifies multiple subscriptions") {
		Observable o = Observable::just("Hello");
		RxJUCECollectItems(o, results);
		RxJUCECollectItems(o, results);
		
		RxJUCERequireResults(results, "Hello", "Hello");
	}
}


TEST_CASE("Observable::range",
		  "[Observable][Observable::range]")
{
	Array<var> results;
	
	IT("emits integer numbers with an integer range") {
		RxJUCECollectItems(Observable::range(Range<int>(3, 7), 3), results);
		RxJUCERequireResults(results, 3, 6, 7);
	}
	
	IT("emits double numbers with a double range") {
		RxJUCECollectItems(Observable::range(Range<double>(17.5, 22.8), 2), results);
		RxJUCERequireResults(results, 17.5, 19.5, 21.5, 22.8);
	}
	
	IT("emits just start if start == end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 10), 1), results);
		RxJUCERequireResults(results, 10);
	}
	
	IT("emits just start if start > end") {
		RxJUCECollectItems(Observable::range(Range<int>(10, 9), 1), results);
		RxJUCERequireResults(results, 10);
	}
}


TEST_CASE("Observable::fromValue",
		  "[Observable][Observable::fromValue]")
{
	Value value("Initial Value");
	const auto observable = Observable::fromValue(value);
	Array<var> results;
	RxJUCECollectItems(observable, results);
	
	RxJUCECheckResults(results, "Initial Value");
	
	IT("emits if a copy of the Value sets a new value") {
		Value copy(value);
		copy.setValue("Set by copy");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial Value", "Set by copy");
	}
	
	IT("emites only one item if the Value is set multiple times synchronously") {
		value = "2";
		value = "3";
		value = "4";
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial Value", "4");
	}
	
	IT("notifies multiple Subscriptions on subscribe") {
		Observable another = Observable::fromValue(value);
		RxJUCECollectItems(another, results);
		
		RxJUCERequireResults(results, "Initial Value", "Initial Value");
	}
	
	IT("notifies multiple Values referring to the same ValueSource") {
		Value anotherValue(value);
		Observable anotherObservable = Observable::fromValue(anotherValue);
		RxJUCECollectItems(anotherObservable, results);
		
		RxJUCERequireResults(results, "Initial Value", "Initial Value");
	}
	
	IT("notifies multiple Subscriptions if a Value is set multiple times") {
		ScopedSubscription another = observable.subscribe([&](String newValue) {
			results.add(newValue.toUpperCase());
		});
		
		value = "Bar";
		RxJUCERunDispatchLoop();
		
		value = "Baz";
		RxJUCERunDispatchLoop();
		
		CHECK(results.size() == 6);
		
		// Subscribers are notified in no particular order
		for (auto s : {"Initial Value", "INITIAL VALUE", "BAR", "Bar", "BAZ", "Baz"})
			REQUIRE(results.contains(s));
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
	Array<var> results;
	RxJUCECollectItems(mapped, results);
	
	RxJUCECheckResults(results, "Initial");
	
	IT("emits items when the source Observable is alive") {
		value.setValue("New Value");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial", "New Value");
	}
	
	IT("stops emitting items as soon as the source Observable is destroyed") {
		source.reset();
		value.setValue("Two");
		value.setValue("Three");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial");
	}
	
	IT("does not emit an item if the Observable is destroyed immediately after calling setValue") {
		value.setValue("New Value");
		source.reset();
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, "Initial");
	}
	
	IT("continues to emit items if the source Observable is copied and then destroyed") {
		auto copy = std::make_shared<Observable>(*source);
		Array<var> copyResults;
		RxJUCECollectItems(*copy, copyResults);
		
		RxJUCECheckResults(copyResults, "Initial");
		
		source.reset();
		RxJUCERunDispatchLoop();
		value.setValue("New");
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(copyResults, "Initial", "New");
	}
}


TEST_CASE("Observable::fromValue with a Slider",
		  "[Observable][Observable::fromValue]")
{
	Slider slider;
	slider.setValue(7.6);
	Observable o = Observable::fromValue(slider.getValueObject());
	Array<var> results;
	RxJUCECollectItems(o, results);
	RxJUCECheckResults(results, 7.6);
	
	IT("emits once if the Slider is changed once") {
		slider.setValue(0.45);
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, 7.6, 0.45);
	}
	
	IT("emits just once if the Slider value changes rapidly") {
		for (double value : {3.41, 9.54, 4.67, 3.56})
			slider.setValue(value);
		
		RxJUCERunDispatchLoop();
		
		RxJUCERequireResults(results, 7.6, 3.56);
	}
}


TEST_CASE("Observable::create",
		  "[Observable][Observable::create]")
{
	Array<var> results;
	
	IT("emits items when pushing items synchronously") {
		auto observable = Observable::create([](Observer observer) {
			observer.onNext("First");
			observer.onNext("Second");
		});
		RxJUCECollectItems(observable, results);
		
		RxJUCERequireResults(results, "First", "Second");
	}
	
	IT("emits items when pushing items asynchronously") {
		auto observable = Observable::create([](Observer observer) {
			MessageManager::getInstance()->callAsync([observer]() {
				observer.onNext("First");
				observer.onNext("Second");
			});
		});
		RxJUCECollectItems(observable, results);
		
		// There shouldn't be any items until the async callback is executed
		CHECK(results.isEmpty());
		
		// The items should be there after running the dispatch loop
		RxJUCERunDispatchLoop();
		RxJUCERequireResults(results, "First", "Second");
	}
	
	IT("emits can emit items asynchronously after being destroyed") {
		auto observable = std::make_shared<Observable>(Observable::create([](Observer observer) {
			MessageManager::getInstance()->callAsync([observer]() {
				observer.onNext("First");
				observer.onNext("Second");
			});
		}));
		
		IT("emits when there's still a subscription") {
			auto subscription = observable->subscribe([&](var next){ results.add(next); });
			observable.reset();
			RxJUCERunDispatchLoop();
			
			RxJUCERequireResults(results, "First", "Second");
		}
		
		IT("doesn't emit when the subscription has unsubscribed") {
			auto subscription = observable->subscribe([&](var next){ results.add(next); });
			observable.reset();
			subscription.unsubscribe();
			RxJUCERunDispatchLoop();
			
			REQUIRE(results.isEmpty());
		}
		
	}
	
	IT("calls onSubscribe again for each new subscription") {
		auto observable = Observable::create([](Observer observer) {
			observer.onNext("onSubscribe called");
		});
		RxJUCECollectItems(observable, results);
		RxJUCECollectItems(observable, results);
		RxJUCECollectItems(observable, results);
		
		RxJUCERequireResults(results, "onSubscribe called", "onSubscribe called", "onSubscribe called");
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
	Array<var> results;
	auto source = Observable::range(Range<double>(4, 7), 2);
	
	IT("emits values synchronously") {
		auto mapped = source.map([](int i) { return i * 1.5; });
		RxJUCECollectItems(mapped, results);
		
		RxJUCERequireResults(results, 6.0, 9.0, 10.5);
	}
}


TEST_CASE("Interaction between Observable::map and Observable::switchOnNext",
		  "[Observable][Observable::map][Observable::switchOnNext]")
{
	Array<var> results;
	
	IT("supports returning Observables in map, even nested twice") {
		auto source = Observable::just(1);
		auto nested = source.map([](int i) {
			return Observable::just("Hello").map([i](String s) {
				return Observable::just(String(i) + " " + s);
			});
		});
		
		// Unwrap twice
		nested = nested.switchOnNext().switchOnNext();
		RxJUCECollectItems(nested, results);
		
		RxJUCERequireResults(results, "1 Hello");
	}
	
	IT("continues to emit items after the source Observable is gone") {
		auto source = std::make_shared<Observable>(Observable::just(17));
		auto mapped = source->map([](int next) {
			return Observable::create([next](Observer observer) {
				MessageManager::getInstance()->callAsync([observer, next]() {
					observer.onNext(next * 3);
				});
			});
		});
		mapped = mapped.switchOnNext();
		RxJUCECollectItems(mapped, results);
		
		// There should be no items before running dispatch loop
		CHECK(results.isEmpty());
		
		source.reset();
		RxJUCERunDispatchLoop();
		
		// The item should be emitted, although there's no reference to the source anymore
		RxJUCERequireResults(results, 17 * 3);
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
	Array<var> results;
	OwnedArray<Observable> os;
	for (int i = 0; i < 8; i++)
		os.add(new Observable(Observable::just(String(i) + " ")));
	
	IT("works with arity 1") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], transform<var, var>), results);
		RxJUCERequireResults(results, "0 1 ");
	}
	
	IT("works with arity 2") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], transform<var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 ");
	}
	
	IT("works with arity 3") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], transform<var, var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 3 ");
	}
	
	IT("works with arity 4") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], transform<var, var, var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 3 4 ");
	}
	
	IT("works with arity 5") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], transform<var, var, var, var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 3 4 5 ");
	}
	
	IT("works with arity 6") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], *os[6], transform<var, var, var, var, var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 3 4 5 6 ");
	}
	
	IT("works with arity 7") {
		RxJUCECollectItems(os[0]->combineLatest(*os[1], *os[2], *os[3], *os[4], *os[5], *os[6], *os[7], transform<var, var, var, var, var, var, var, var>), results);
		RxJUCERequireResults(results, "0 1 2 3 4 5 6 7 ");
	}
}
