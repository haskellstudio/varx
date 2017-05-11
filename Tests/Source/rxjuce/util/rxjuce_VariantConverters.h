/*
  ==============================================================================

    rxjuce_VariantConverters.h
    Created: 10 May 2017 9:24:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"
#include "rxjuce_Observable.h"

namespace juce {
	template<>
	struct VariantConverter<rxjuce::Observable>
	{
		static rxjuce::Observable fromVar(const var &v);
		static var toVar(const rxjuce::Observable& observable);
	};
	
	template<>
	struct VariantConverter<Button::ButtonState>
	{
		static Button::ButtonState fromVar(const var &v);
		static var toVar(const Button::ButtonState& buttonState);
	};
}
