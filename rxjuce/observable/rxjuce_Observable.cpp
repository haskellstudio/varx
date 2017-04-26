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
	
	static Ptr fromRxCpp(const rxcpp::observable<var>& o)
	{
		return Ptr(new Internal(o));
	}
	
	template<typename Transform, typename O, typename... Os>
	static Ptr combineLatest(Transform&& transform, O&& observable, Os... observables)
	{
		return fromRxCpp(observable.internal->o.combine_latest(transform, observables.internal->o...));
	}
	
	template<typename T>
	static Ptr range(var first, var last, int step)
	{
		return fromRxCpp(rxcpp::observable<>::range<T>(first, last, step).map(VariantConverter<T>::toVar));
	}
	
	const rxcpp::observable<var> o;
	
private:
	Internal(const rxcpp::observable<var>& o)
	:	o(o)
	{}
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Internal)
};

Observable Observable::fromValue(Value value)
{
	class ValueListener : private Value::Listener
	{
	public:
		ValueListener(Value::ValueSource &source, const std::function<void(var)>& valueChanged)
		:	value(&source),
			_valueChanged(valueChanged)
		{
			value.addListener(this);
		}
		
		bool sourceHasExpired()
		{
			return (value.getValueSource().getReferenceCount() == 1);
		}
		
	private:
		void valueChanged(Value &value) override
		{
			_valueChanged(value.getValue());
		}
		
		Value value;
		const std::function<void(var)> _valueChanged;
		
		JUCE_DECLARE_NON_COPYABLE(ValueListener)
	};
	
	class ValueListenerPool : private Timer
	{
	public:
		ValueListenerPool() {}
		
		void add(ValueListener *listener)
		{
			{
				// The timerCallback is called on the message thread. This may be called from a background thread, in which case the message manager must be locked.
				const MessageManagerLock lock(Thread::getCurrentThread());
				if (!lock.lockWasGained())
					return; // Some other thread is trying to kill this thread
				
				listeners.add(listener);
			}
			startTimerHz(60);
		}
		
	private:
		void timerCallback() override
		{
			for (size_t i = 0; i < listeners.size();) {
				if (listeners[i]->sourceHasExpired())
					listeners.remove(i);
				else
					i += 1;
			}
			
			if (listeners.isEmpty())
				stopTimer();
		}
		
		OwnedArray<ValueListener> listeners;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueListenerPool)
	};
	
	static ValueListenerPool pool;
	
	return create([value](Subscriber s) mutable {
		// Send the current value
		s.onNext(value.getValue());
		
		// Called when the value is changed
		const auto onValueChanged = [s](const var& newValue) {
			s.onNext(newValue);
		};
		
		// Create the value listener
		ScopedPointer<ValueListener> listener(new ValueListener(value.getValueSource(), onValueChanged));
		
		// Add the listener to the pool, to keep getting updates from the value source
		pool.add(listener.release());
	});
}

Observable::Observable(const shared_ptr<Internal>& internal)
:	internal(internal)
{}

Subscription Observable::subscribe(const std::function<void(var)>& f) const
{
	const auto subscription = internal->o.subscribe(f);
	
	return Subscription([subscription]() {
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
	return Internal::fromRxCpp(internal->o.map(transform));
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
