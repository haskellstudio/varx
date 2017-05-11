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

ButtonConnection::ButtonConnection(Button& parent)
: ComponentConnection(parent),
  clicked(_clicked.getObservable()),
  buttonState(parent.getState())
{
	parent.addListener(this);
	
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

ValueConnection::ValueConnection(const juce::Value& parent)
: observable(Observable::fromValue(parent)) {}

RXJUCE_NAMESPACE_END
