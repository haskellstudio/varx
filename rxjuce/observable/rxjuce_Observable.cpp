//
//  rxjuce_Observable.cpp
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

class Observable::Internal
{
public:
	typedef juce::Array<std::shared_ptr<void>> Sources;
	typedef std::shared_ptr<Internal> Ptr;
	
	static Ptr create(const rxcpp::observable<juce::var>& o, const std::initializer_list<std::shared_ptr<void>>& sources = {})
	{
		return std::make_shared<Internal>(o, sources);
	}
	
	static Ptr fromValue(const juce::Value& value)
	{
		class ValueListeningInternal : public Internal, private juce::Value::Listener
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
	
	Internal(const rxcpp::observable<juce::var>& o = rxcpp::observable<>::never<juce::var>(), const juce::Array<std::shared_ptr<void>>& sources = {})
	:	o(o),
	sources(sources)
	{}
	
	~Internal() {}
	
	template<typename Transform, typename O, typename... Os>
	static Ptr combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		return create(observable.internal->o.combine_latest(transform, observables.internal->o...), {observable.internal, observables.internal...});
	}
	
	template<typename T>
	static Ptr range(var first, var last, std::ptrdiff_t step)
	{
		return create(rxcpp::observable<>::range<T>(first, last, step).map(juce::VariantConverter<T>::toVar));
	}
	
	rxcpp::observable<juce::var> o;
	Sources sources;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

Observable::Observable(const juce::Value& value)
:	Observable(Internal::fromValue(value))
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

Observable Observable::just(var value)
{
	return Internal::create(rxcpp::observable<>::just(value));
}


Observable Observable::range(var first, var last, std::ptrdiff_t step)
{
	jassert(first.hasSameTypeAs(last));
	
	std::function<Internal::Ptr(var, var, std::ptrdiff_t)> createRange;
	
	if (first.isInt())
		createRange = Internal::range<int>;
	else if (first.isInt64())
		createRange = Internal::range<juce::int64>;
	else if (first.isDouble())
		createRange = Internal::range<double>;
	else
		jassertfalse;
	
	return createRange(first, last, step);
}

Observable Observable::map(Transform1 transform)
{
	return Internal::create(internal->o.map(transform), {internal});
}

Observable Observable::combineLatest(Observable o1, Transform2& transform)
{
	return Internal::combineLatest(transform, *this, o1);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Transform3 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Transform4 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Transform5 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4);
}

#ifdef RXJUCE_ENABLE_LONG_SIGNATURES

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Transform6 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Transform7 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Transform8 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6, o7);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Observable o8, Transform9 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6, o7, o8);
}

Observable Observable::combineLatest(Observable o1, Observable o2, Observable o3, Observable o4, Observable o5, Observable o6, Observable o7, Observable o8, Observable o9, Transform10 transform)
{
	return Internal::combineLatest(transform, *this, o1, o2, o3, o4, o5, o6, o7, o8, o9);
}

#endif

Observable Observable::combineLatest(const juce::Array<Observable>& others, const std::function<juce::var(const juce::Array<juce::var>&)>& transform)
{
	static const auto wrapIntoArray = [](const juce::var& v) {
		juce::Array<juce::var> array;
		array.add(v);
		return array;
	};
	
	Observable o = map(wrapIntoArray);
	
	static const auto combine = [](const juce::var& array, const juce::var& other) {
		array.getArray()->add(other);
		return array;
	};
	
	for (const auto& other : others)
		o = Internal::combineLatest(combine, o, other);
	
	return o.map([transform](const juce::var& array){ return transform(*array.getArray()); });
}
