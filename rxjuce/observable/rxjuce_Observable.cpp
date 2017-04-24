//
//  rxjuce_Observable.cpp
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#include "../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

#include "rxjuce_Observable.h"

#include <vector>


namespace rxjuce {
	class Observable::Internal : public juce::ReferenceCountedObject
	{
	public:
		typedef std::vector<std::shared_ptr<Observable::Internal>> Sources;
		Internal(const rxcpp::observable<juce::var>& o, const Sources& sources)
		:	o(o),
			sources(sources)
		{}
		Internal() {}
		~Internal() {}
		
		rxcpp::observable<juce::var> o;
		Sources sources;
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
		return std::make_shared<Internal>(rxcpp::observable<>::just(value), Internal::Sources());
	}
	
	Observable Observable::map(const std::function<juce::var(juce::var)>& transform)
	{
		return std::make_shared<Internal>(internal->o.map(transform), Internal::Sources({internal}));
	}
	
	Observable Observable::combineLatest(juce::Array<Observable> others, const std::function<juce::var(const juce::Array<juce::var>&)>& transform)
	{
		Observable o = map([](const juce::var& v) {
			juce::Array<juce::var> array;
			array.add(v);
			return array;
		});
		
		for (const auto& other : others) {
			const auto combine = [](const juce::var& array, const juce::var& other) {
				array.getArray()->add(other);
				return array;
			};
			o = std::make_shared<Internal>(o.internal->o.combine_latest(combine, other.internal->o), Internal::Sources({other.internal}));
		}
		
		return o.map([transform](const juce::var& array){ return transform(*array.getArray()); });
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
