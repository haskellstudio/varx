/*
  ==============================================================================

    rxjuce_Connections.cpp
    Created: 11 May 2017 8:15:44am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Connections.h"
#include "rxjuce_VariantConverters.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

ValueConnection::ValueConnection(const Value& inputValue)
: subject(inputValue.getValue()),
value(inputValue)
{
	value.addListener(this);
	subscriptions.add(subject.subscribe([this](const var& next) {
		value.setValue(next);
	}));
}

void ValueConnection::valueChanged(Value&)
{
	subject.onNext(value.getValue());
}

ComponentConnection::ComponentConnection(Component& parent)
: visible(parent.isVisible()),
  beingDeleted(_beingDeleted.getObservable()) {}

void ComponentConnection::componentVisibilityChanged(Component& component)
{
	const bool latestItem = visible.getLatestItem();
	if (component.isVisible() != latestItem) {
		visible.onNext(component.isVisible());
	}
}

void ComponentConnection::componentBeingDeleted(Component& component)
{
	_beingDeleted.onNext(var());
}


ButtonConnection::ButtonConnection(Button& parent)
: ComponentConnection(parent),
  _toggleState(parent.getToggleStateValue()),
  clicked(_clicked.getObservable()),
  buttonState(parent.getState()),
  toggleState(_toggleState.subject),
  text(_text.getObserver()),
  tooltip(_tooltip.getObserver())
{
	parent.addListener(this);
	
	subscriptions.add(_text.subscribe([](const String& text) {
		
	}));
	
	subscriptions.add(buttonState.subscribe([&parent](const var& v) {
		parent.setState(VariantConverter<Button::ButtonState>::fromVar(v));
	}));
}

void ButtonConnection::buttonClicked(Button *)
{
	_clicked.onNext(var());
}

void ButtonConnection::buttonStateChanged(Button *button)
{
	if (var(button->getState()) != buttonState.getLatestItem())
		buttonState.onNext(button->getState());
}

RXJUCE_NAMESPACE_END
