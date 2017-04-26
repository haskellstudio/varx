#include "rxjuce.h"

#include "RxCpp/Rx/v2/src/rxcpp/rx.hpp"

#include <utility>
#include <initializer_list>

namespace rxjuce {
	using namespace juce;
	
	template<class T>
	using shared_ptr = std::shared_ptr<T>;

#include "observable/rxjuce_Observable.cpp"
#include "observable/rxjuce_Subscriber.cpp"
#include "observable/rxjuce_Subscription.cpp"

}
