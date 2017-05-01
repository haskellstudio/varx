/*
  ==============================================================================

    rxjuce_Observed.cpp
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observed.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

detail::ButtonForwarder::ButtonForwarder(Button& button)
: buttonState(button.getState())
{
	button.addListener(this);
}

Observable detail::ButtonForwarder::clickedObservable() const
{
	return clicked.getObservable();
}

Observable detail::ButtonForwarder::buttonStateObservable() const
{
	return buttonState.getObservable();
}

void detail::ButtonForwarder::buttonClicked(Button *)
{
	clicked.onNext(var());
}

void detail::ButtonForwarder::buttonStateChanged(Button *button)
{
	buttonState.onNext(button->getState());
}


Observed<Value>::Observed()
: observable(Observable::fromValue(*this)) {}

Observed<Value>::Observed(const Value& other)
: Value(other),
  observable(Observable::fromValue(*this)) {}

Observed<Value>::Observed(const var& initialValue)
: Value(initialValue),
  observable(Observable::fromValue(*this)) {}

Observed<Value>::operator var() const
{
	return Value::operator var();
}

Observed<Value>& Observed<Value>::operator=(const var& newValue)
{
	Value::operator=(newValue); // Causes Observable to emit
	return *this;
}

Observable Observed<Value>::getObservable() const
{
	return observable;
}

RXJUCE_NAMESPACE_END
