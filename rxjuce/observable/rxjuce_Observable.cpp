//
//  rxjuce_Observable.cpp
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

#include "rxjuce_Observable.h"


namespace rxjuce {
	class Observable::Internal : public juce::ReferenceCountedObject
	{
	public:
		Internal(const rxcpp::observable<juce::var>& o)
		:	o(o)
		{}
		Internal() {}
		~Internal() {}
		
		rxcpp::observable<juce::var> o;
	};
	
	Observable::Observable(const juce::Value& value)
	:	Observable(InternalFromValue(value))
	{}
	
	Observable::Observable(const std::shared_ptr<Internal>& internal)
	:	internal(internal)
	{}
	
	Subscription Observable::subscribe(const std::function<void(juce::var)>& f)
	{
		const auto _internal = internal;
		const auto subscription = _internal->o.subscribe(f);
		
		return Subscription([_internal, subscription]() {
			subscription.unsubscribe();
		});
	}
	
	Observable Observable::just(juce::var value)
	{
		return std::make_shared<Internal>(rxcpp::observable<>::just(value));
	}
	
	Observable Observable::map(const std::function<juce::var(juce::var)>& transform)
	{
		return std::make_shared<Internal>(internal->o.map(transform));
	}
	
	Observable::Internal_Ptr Observable::InternalFromValue(const juce::Value& value)
	{
		class ValueListeningInternal : public Observable::Internal, private juce::Value::Listener
		{
		public:
			ValueListeningInternal(const juce::Value &source)
			:	value(source),
				s(value.getValue())
			{
				o = s.get_observable();
				value.addListener(this);
			}
			
		private:
			void valueChanged(juce::Value &value) override
			{
				s.get_subscriber().on_next(value);
			}
			
			juce::Value value;
			rxcpp::subjects::behavior<juce::var> s;
		};
		
		return std::make_shared<ValueListeningInternal>(value);
	}
}
