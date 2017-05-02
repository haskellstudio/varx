/*
  ==============================================================================

    OperatorsTest.cpp
    Created: 2 May 2017 10:27:38pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_TestPrefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"


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


TEST_CASE("Observable::filter",
		  "[Observable][Observable::filter]")
{
	Array<var> items;
	auto source = Observable::range(Range<double>(4, 9), 1);
	
	IT("filters ints") {
		auto source = Observable::range(Range<int>(4, 9), 1);
		auto filtered = source.filter([](int i) {
			return (i % 2 == 0);
		});
		RxJUCECollectItems(filtered, items);
		
		RxJUCERequireItems(items, 4, 6, 8);
	}
	
	IT("filters Strings") {
		auto source = Observable::from({"Hello", "Great", "World", "Hey"});
		auto filtered = source.filter([](String s) {
			return s[0] == 'H';
		});
		RxJUCECollectItems(filtered, items);
		
		RxJUCERequireItems(items, "Hello", "Hey");
	}
	
	IT("filters an Observable which emits different types") {
		auto source = Observable::from({var(3), var("Hello"), var(5.43)});
		auto filtered = source.filter([](var v) {
			return v.isDouble();
		});
		RxJUCECollectItems(filtered, items);
		
		RxJUCERequireItems(items, 5.43);
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
