/*
  ==============================================================================

    rxjuce_VariantConverters.cpp
    Created: 10 May 2017 9:24:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_VariantConverters.h"

#include "rxjuce_Observable.h"

RXJUCE_SOURCE_PREFIX

namespace juce {
	Button::ButtonState VariantConverter<Button::ButtonState>::fromVar(const var &v)
	{
		return static_cast<Button::ButtonState>(int(v));
	}
	
	var VariantConverter<Button::ButtonState>::toVar(const Button::ButtonState& buttonState)
	{
		return var(buttonState);
	}
}
