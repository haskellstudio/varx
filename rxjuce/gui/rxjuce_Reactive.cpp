/*
  ==============================================================================

    rxjuce_Reactive.cpp
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/

Reactive<Value>::Reactive()
: rx(*this) {}

Reactive<Value>::Reactive(const Value& other)
: Value(other),
  rx(*this) {}

Reactive<Value>::Reactive(const var& initialValue)
: Value(initialValue),
  rx(*this) {}

Reactive<Value>& Reactive<Value>::operator=(const var& newValue)
{
	Value::operator=(newValue); // Causes Observable to emit
	return *this;
}


