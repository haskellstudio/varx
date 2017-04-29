/*
  ==============================================================================

    rxjuce_Observable_Internal.cpp
    Created: 27 Apr 2017 7:11:30am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observable_Internal.h"

#include "rxjuce_Subscriber.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

std::shared_ptr<Observable::Internal> Observable::Internal::fromRxCpp(const rxcpp::observable<var>& o)
{
	auto internal = std::make_shared<Internal>();
	internal->o = o;
	return internal;
}

std::shared_ptr<Observable::Internal> Observable::Internal::fromValue(const Value& value)
{
	class ValueObservableImpl : public Internal, private Value::Listener {
	public:
		ValueObservableImpl(Value inputValue)
		: Internal(),
		  value(inputValue),
		  subject(inputValue.getValue())
		{
			o = this->subject.get_observable();
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

Observable::Internal::Internal()
{}

RXJUCE_NAMESPACE_END
