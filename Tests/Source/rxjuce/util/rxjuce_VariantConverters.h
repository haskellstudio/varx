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

RXJUCE_NAMESPACE_BEGIN

namespace detail {
	template<typename T>
	struct ReferenceCountingVariantConverter
	{
	public:
		static T fromVar(const juce::var& v)
		{
			juce::ReferenceCountedObjectPtr<juce::ReferenceCountedObject> ptr(v.getObject());
			
			if (auto wrapper = dynamic_cast<ReferenceCountedWrapper *>(ptr.get()))
				return wrapper->t;
			else
				throw std::runtime_error("Error unwrapping type from var. You might be trying to extract a different type than the type that was inserted.");
		}
		
		static juce::var toVar(const T& t)
		{
			return new ReferenceCountedWrapper(t);
		}
		
	private:
		// Wraps a copyable type as a ReferenceCountedObject, so it can be stored in a juce var
		struct ReferenceCountedWrapper : public juce::ReferenceCountedObject
		{
			ReferenceCountedWrapper(const T& t)
			: t(t) {}
			
			const T t;
		};
	};
}

template<typename T>
T fromVar(const juce::var& v)
{
	return juce::VariantConverter<T>::fromVar(v);
}

template<typename T>
juce::var toVar(const T& t)
{
	return juce::VariantConverter<T>::toVar(t);
}

RXJUCE_NAMESPACE_END

namespace juce {
	template<>
	struct VariantConverter<rxjuce::Observable> : public rxjuce::detail::ReferenceCountingVariantConverter<rxjuce::Observable> {};
	
	template<>
	struct VariantConverter<juce::Image> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::Image> {};
	
	template<>
	struct VariantConverter<Button::ButtonState>
	{
		static Button::ButtonState fromVar(const var &v);
		static var toVar(const Button::ButtonState& buttonState);
	};
}
