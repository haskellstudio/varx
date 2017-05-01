/*
  ==============================================================================

    rxjuce_Observable_Impl.cpp
    Created: 27 Apr 2017 7:11:30am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observable_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

shared_ptr<Observable::Impl> Observable::Impl::fromRxCpp(const rxcpp::observable<var>& wrapped)
{
	return std::make_shared<Impl>(wrapped);
}

shared_ptr<Observable::Impl> Observable::Impl::fromValue(const Value& value)
{
	// An Observable::Impl that holds a Value to keep receiving changes until the Observable is destroyed.
	class ValueObservableImpl : public Impl, private Value::Listener {
	public:
		ValueObservableImpl(const Value& inputValue)
		: value(inputValue),
		  subject(inputValue)
		{
			wrapped = subject.get_observable();
			value.addListener(this);
		}
		
		~ValueObservableImpl()
		{
			value.removeListener(this);
			subject.get_subscriber().on_completed();
		}
		
		void valueChanged(Value &newValue) override
		{
			subject.get_subscriber().on_next(newValue);
		}
		
	private:
		Value value;
		const rxcpp::subjects::behavior<var> subject;
	};

	return std::make_shared<ValueObservableImpl>(value);
}

Observable::Impl::Impl(const rxcpp::observable<var>& wrapped)
: wrapped(wrapped)
{}

RXJUCE_NAMESPACE_END
