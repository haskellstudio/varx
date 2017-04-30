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

ButtonForwarder::ButtonForwarder(Button& button)
: buttonState(button.getState())
{
	button.addListener(this);
}

Observable ButtonForwarder::clickedObservable() const
{
	return clicked.getObservable();
}

Observable ButtonForwarder::buttonStateObservable() const
{
	return buttonState.getObservable();
}

void ButtonForwarder::buttonClicked(Button *)
{
	clicked.onNext(var());
}

void ButtonForwarder::buttonStateChanged(Button *button)
{
	buttonState.onNext(var(button->getState()));
}


Observable Observed<Value>::getObservable() const
{
	return observable;
}

RXJUCE_NAMESPACE_END
