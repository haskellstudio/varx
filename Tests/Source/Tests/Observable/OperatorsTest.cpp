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
#include "rxjuce_Subjects.h"


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


TEST_CASE("Observable::concat",
		  "[Observable][Observable::concat]")
{
	Array<var> items;
	
	IT("concatenates the values emitted by the source Observables") {
		auto observable = Observable::from({"Hello", "World"});
		auto another = Observable::from({1.5, 2.32, 5.6});
		RxJUCECollectItems(observable.concat(another), items);
		
		RxJUCERequireItems(items, var("Hello"), var("World"), var(1.5), var(2.32), var(5.6));
	}
}


TEST_CASE("Observable::distinctUntilChanged",
		  "[Observable][Observable::distinctUntilChanged]")
{
	IT("doesn't emit consecutive duplicate items") {
		Array<var> originalItems;
		Array<var> filteredItems;
		PublishSubject subject;
		
		RxJUCECollectItems(subject, originalItems);
		RxJUCECollectItems(subject.distinctUntilChanged(), filteredItems);
		
		subject.onNext(3);
		subject.onNext(3);
		subject.onNext("3"); // Is equal to 3, due to how juce::var defines operator==
		subject.onNext(3);
		subject.onNext(3);
		subject.onNext(5);
		subject.onNext(3);
		
		RxJUCERequireItems(originalItems, var(3), var(3), var("3"), var(3), var(3), var(5), var(3));
		RxJUCERequireItems(filteredItems, var(3), var(5), var(3));
	}
}


TEST_CASE("Observable::elementAt",
		  "[Observable][Observable::elementAt]")
{
	auto observable = Observable::from({17.4, 3.0, 1.5, 6.77});
	Array<var> items;
	
	IT("emits only the item at the given index") {
		RxJUCECollectItems(observable.elementAt(2), items);
		
		RxJUCERequireItems(items, 1.5);
	}
}


TEST_CASE("Observable::filter",
		  "[Observable][Observable::filter]")
{
	Array<var> items;
	auto source = Observable::range(4, 9, 1);
	
	IT("filters ints") {
		auto source = Observable::range(4, 9, 1);
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


TEST_CASE("Observable::flatMap",
		  "[Observable][Observable::flatMap]")
{
	Array<var> items;
	
	IT("merges the values emitted by the returned Observables") {
		auto o = Observable::from({"Hello", "World"}).flatMap([](String s) {
			return Observable::from({s.toLowerCase(), s.toUpperCase() + "!"});
		});
		RxJUCECollectItems(o, items);
		
		RxJUCERequireItems(items, "hello", "HELLO!", "world", "WORLD!");
	}
}


TEST_CASE("Observable::map",
		  "[Observable][Observable::map]")
{
	Array<var> items;
	auto source = Observable::range(4, 7, 2);
	
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
		DisposeBag disposeBag;
		o.subscribe([](var) {}, onError).disposedBy(disposeBag);
		
		REQUIRE(onErrorCalled);
	}
}


TEST_CASE("Observable::reduce",
		  "[Observable][Observable::reduce]")
{
	Array<var> items;
	
	IT("reduces emitted items") {
		auto observable = Observable::from({10, 100, 1000}).reduce(2, [](int accum, int next) {
			return accum + next;
		});

		RxJUCECollectItems(observable, items);
		
		RxJUCERequireItems(items, 1112);
	}
}


TEST_CASE("Observable::scan",
		  "[Observable][Observable::scan]")
{
	Array<var> items;
	
	IT("applies the transform function to the inputs") {
		auto o = Observable::range(1, 5).scan(10, [](int accum, int currentValue) {
			return accum + currentValue;
		});
		RxJUCECollectItems(o, items);
		
		RxJUCERequireItems(items, 11, 13, 16, 20, 25);
	}
}


TEST_CASE("Observable::skip",
		  "[Observable][Observable::skip]")
{
	Array<var> items;
	
	IT("skips the first 4 items") {
		auto o = Observable::from({4, 7, 2, 1, 19, 1, 33, 4}).skip(4);
		RxJUCECollectItems(o, items);
		
		RxJUCERequireItems(items, 19, 1, 33, 4);
	}
}


TEST_CASE("Observable::skipUntil",
		  "[Observable][Observable::skipUntil]")
{
	Array<var> items;
	
	IT("skips until another Observable emits an item") {
		PublishSubject subject;
		PublishSubject trigger;
		
		RxJUCECollectItems(subject.skipUntil(trigger), items);
		
		// Emit some items, these should NOT be received
		subject.onNext("Not");
		subject.onNext("Getting");
		subject.onNext("This");
		
		// Trigger
		trigger.onNext(var::undefined());
		
		// Emit more items, these should be received
		subject.onNext("These");
		subject.onNext("Are");
		subject.onNext("Received");
		
		RxJUCERequireItems(items, "These", "Are", "Received");
	}
}


TEST_CASE("Observable::startWith",
		  "[Observable][Observable::startWith]")
{
	Array<var> items;
	auto observable = Observable::from({17, 3});
	
	IT("prepends items to an existing Observable") {
		RxJUCECollectItems(observable.startWith(6, 4, 7, 2), items);
		
		RxJUCERequireItems(items, 6, 4, 7, 2, 17, 3);
	}
}


TEST_CASE("Observable::takeLast",
		  "[Observable][Observable::takeLast]")
{
	Array<var> items;
	auto observable = Observable::from({"First", "Another", "And one more", "Last item"});
	
	IT("takes the last 2 emitted items") {
		RxJUCECollectItems(observable.takeLast(2), items);
		
		RxJUCERequireItems(items, "And one more", "Last item");
	}
}


TEST_CASE("Observable::takeUntil",
		  "[Observable][Observable::takeUntil]")
{
	Array<var> items;
	
	IT("emits until another Observable emits an item") {
		PublishSubject subject;
		PublishSubject trigger;
		
		RxJUCECollectItems(subject.takeUntil(trigger), items);
		
		// Emit some items, these should be received
		subject.onNext("These");
		subject.onNext("Are");
		subject.onNext("Received");
		
		// Trigger
		trigger.onNext("Hey stop!");
		
		// Emit more items, these should NOT be received
		subject.onNext("Not");
		subject.onNext("Getting");
		subject.onNext("This");
		
		RxJUCERequireItems(items, "These", "Are", "Received");
	}
}


TEST_CASE("Observable::takeWhile",
		  "[Observable][Observable::takeWhile]")
{
	Array<var> items;
	
	IT("emits items as long as the predicate returns true") {
		PublishSubject subject;
		const auto predicate = [](int i) {
			return i <= 10;
		};
		
		RxJUCECollectItems(subject.takeWhile(predicate), items);
		
		// These should be emitted
		subject.onNext(4);
		subject.onNext(7);
		subject.onNext(10);
		
		// These shouldn't be emitted, because predicate(11) == false
		subject.onNext(11);
		subject.onNext(3);
		subject.onNext(7);
		
		RxJUCERequireItems(items, 4, 7, 10);
	}
}


TEST_CASE("Observable::zip",
		  "[Observable][Observable::zip]")
{
	Array<var> items;
	
	IT("zips three Observables together") {
		PublishSubject strings;
		PublishSubject ints;
		PublishSubject doubles;
		const auto combine = [](String s, int i, double d) {
			return "s=" + s + "; i=" + String(i) + "; d=" + String(d);
		};
		
		RxJUCECollectItems(strings.zip(ints, doubles, combine), items);
		
		// First item should be emitted when all three Observables have emitted once
		strings.onNext("a");
		CHECK(items.isEmpty());
		ints.onNext(1);
		CHECK(items.isEmpty());
		doubles.onNext(0.1);
		RxJUCECheckItems(items, "s=a; i=1; d=0.1");
		
		// Second item should be emitted when all three Observables have emitted twice
		doubles.onNext(0.25);
		CHECK(items.size() == 1);
		ints.onNext(57);
		CHECK(items.size() == 1);
		strings.onNext("x");
		RxJUCERequireItems(items, "s=a; i=1; d=0.1", "s=x; i=57; d=0.25");
	}
}
