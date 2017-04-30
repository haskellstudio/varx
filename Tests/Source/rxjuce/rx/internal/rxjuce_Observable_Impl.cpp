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
	class ValueObservableImpl : public Impl, private Value::Listener {
	public:
		ValueObservableImpl(const Value& inputValue)
		: value(inputValue),
		  subject(inputValue.getValue())
		{
			wrapped = subject.get_observable();
			value.addListener(this);
		}
		
		void valueChanged(Value &value) override
		{
			subject.get_subscriber().on_next(value.getValue());
		}
	private:
		Value value;
		rxcpp::subjects::behavior<var> subject;
	};

	return std::make_shared<ValueObservableImpl>(value);
}

Observable::Impl::Impl(const rxcpp::observable<var>& wrapped)
: wrapped(wrapped)
{}

RXJUCE_NAMESPACE_END
