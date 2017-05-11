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

namespace {
	// Wraps an Observable as a ReferenceCountedObject, so it can be stored in a juce var
	struct ReferenceCountedWrapper : public juce::ReferenceCountedObject
	{
		ReferenceCountedWrapper(const rxjuce::Observable& observable)
		: observable(observable) {}
		
		const rxjuce::Observable observable;
	};
	
	const std::runtime_error ObservableUnwrappingError("Error unwrapping Observable. This is likely because you called switchOnNext on an Observable that doesn't emit Observable items.");
	
	const std::runtime_error InvalidRangeError("Invalid range.");
	
	std::chrono::milliseconds durationFromRelativeTime(const juce::RelativeTime& relativeTime)
	{
		return std::chrono::milliseconds(relativeTime.inMilliseconds());
	}
}

namespace juce {
	rxjuce::Observable VariantConverter<rxjuce::Observable>::fromVar(const var &v)
	{
		ReferenceCountedObjectPtr<ReferenceCountedObject> ptr(v.getObject());
		
		if (auto wrapper = dynamic_cast<ReferenceCountedWrapper *>(ptr.get()))
			return wrapper->observable;
		else
			throw ObservableUnwrappingError;
	}
	
	var VariantConverter<rxjuce::Observable>::toVar(const rxjuce::Observable& observable)
	{
		return new ReferenceCountedWrapper(observable);
	}
	
	Button::ButtonState VariantConverter<Button::ButtonState>::fromVar(const var &v)
	{
		return static_cast<Button::ButtonState>(int(v));
	}
	
	var VariantConverter<Button::ButtonState>::toVar(const Button::ButtonState& buttonState)
	{
		return var(buttonState);
	}
}
