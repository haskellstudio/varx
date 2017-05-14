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
			
			if (auto wrapper = dynamic_cast<Wrapped *>(ptr.get()))
				return wrapper->t;
			
			static const std::string ExpectedType = typeid(Wrapped).name();
			std::string actualType;
			auto pointer = ptr.get();
			if (pointer == nullptr) {
				actualType = "nullptr";
			}
			else {
				actualType = typeid(*pointer).name();
			}
			
			throw std::runtime_error("Error unwrapping type from var. Expected: " + ExpectedType + ". Actual: " + actualType + ".");
		}
		
		static juce::var toVar(const T& t)
		{
			return new Wrapped(t);
		}
		
	private:
		// Wraps a copyable type as a ReferenceCountedObject, so it can be stored in a juce var
		struct Wrapped : public juce::ReferenceCountedObject
		{
			Wrapped(const T& t)
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
	struct VariantConverter<juce::Justification> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::Justification> {};
	
	template<>
	struct VariantConverter<juce::RectanglePlacement> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::RectanglePlacement> {};
	
	template<>
	struct VariantConverter<juce::Font> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::Font> {};
	
	template<>
	struct VariantConverter<Button::ButtonState>
	{
		static Button::ButtonState fromVar(const var &v);
		static var toVar(const Button::ButtonState& buttonState);
	};
}
