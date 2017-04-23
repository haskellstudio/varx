//
//  rxjuce_Observable.cpp
//  RxJUCE-Tests
//
//  Created by Martin Finke on 23.04.17.
//
//

#include "rxjuce_Observable.h"

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

namespace rxjuce {
	struct Observable::Impl
	{
		rxcpp::observable<var> o;
	};
	
	Subscription Observable::subscribe(const std::function<void(var)>& f)
	{
		const rxcpp::subscription s = getImpl().o.subscribe(f);
		
		return Subscription([s]() {
			s.unsubscribe();
		});
	}
	
	Observable::Observable(const GetImpl& getImpl)
	: getImpl(getImpl)
	{
	}
	
	Observable Observable::just(var value)
	{
		const auto o = rxcpp::observable<>::just(value);
		return Observable([o]() -> Impl {
			return {.o = o};
		});
	}
}
