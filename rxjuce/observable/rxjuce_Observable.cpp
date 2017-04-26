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
	typedef Array<shared_ptr<void>> Sources;
	typedef shared_ptr<Internal> Ptr;
	
	static Ptr fromRxCpp(const rxcpp::observable<var>& o, const std::initializer_list<shared_ptr<void>>& sources = {})
	{
		return std::make_shared<Internal>(o, sources);
	}
	
	static Ptr fromValueSource(Value::ValueSource &source)
	{
		class ValueListeningInternal : public Internal, private Value::Listener
		{
		public:
			ValueListeningInternal(Value::ValueSource &source)
			:	value(&source),
				s(value.getValue())
			{
				o = s.get_observable();
				value.addListener(this);
			}
			
		private:
			void valueChanged(Value &value) override
			{
				s.get_subscriber().on_next(value);
			}
			
			Value value;
			rxcpp::subjects::behavior<var> s;
		};
		
		return std::make_shared<ValueListeningInternal>(source);
	}
	
	Internal(const rxcpp::observable<var>& o = rxcpp::observable<>::never<var>(), const Array<shared_ptr<void>>& sources = {})
	:	o(o),
	sources(sources)
	{}
	
	~Internal() {}
	
	template<typename Transform, typename O, typename... Os>
	static Ptr combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		return fromRxCpp(observable.internal->o.combine_latest(transform, observables.internal->o...), {observable.internal, observables.internal...});
	}
	
	template<typename T>
	static Ptr range(var first, var last, int step)
	{
		return fromRxCpp(rxcpp::observable<>::range<T>(first, last, step).map(VariantConverter<T>::toVar));
	}
	
	rxcpp::observable<var> o;
	Sources sources;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

Observable Observable::fromValue(const Value& value)
{
	return Internal::fromValueSource(Value(value).getValueSource());
}

Observable::Observable(const shared_ptr<Internal>& internal)
:	internal(internal)
{}

Subscription Observable::subscribe(const std::function<void(var)>& f) const
{
	const auto _internal = internal;
	const auto subscription = _internal->o.subscribe(f);
	
	return Subscription([_internal, subscription]() {
		subscription.unsubscribe();
	});
}

Observable Observable::just(var value)
{
	return Internal::fromRxCpp(rxcpp::observable<>::just(value));
}


Observable Observable::range(var first, var last, int step)
{
	jassert(first.hasSameTypeAs(last));
	
	std::function<Internal::Ptr(var, var, int)> createRange;
	
	if (first.isInt())
		createRange = Internal::range<int>;
	else if (first.isInt64())
		createRange = Internal::range<int64>;
	else if (first.isDouble())
		createRange = Internal::range<double>;
	else
		jassertfalse;
	
	return createRange(first, last, step);
}

Observable Observable::create(const std::function<void(const Subscriber&)>& onSubscribe)
{
	return Internal::fromRxCpp(rxcpp::observable<>::create<var>([onSubscribe](rxcpp::subscriber<var> s) {
		onSubscribe(Subscriber([s](const var& next) {
			s.on_next(next);
		}));
	}));
}

Observable Observable::map(Transform1 transform)
{
	return Internal::fromRxCpp(internal->o.map(transform), {internal});
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

Observable Observable::combineLatest(const Array<Observable>& others, const std::function<var(const Array<var>&)>& transform)
{
	static const auto wrapIntoArray = [](const var& v) {
		Array<var> array;
		array.add(v);
		return array;
	};
	
	Observable o = map(wrapIntoArray);
	
	static const auto combine = [](const var& array, const var& other) {
		array.getArray()->add(other);
		return array;
	};
	
	for (const auto& other : others)
		o = Internal::combineLatest(combine, o, other);
	
	return o.map([transform](const var& array){ return transform(*array.getArray()); });
}
