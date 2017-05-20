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
			
			// Type mismatch. Determine expected and actual type:
			static const std::string ExpectedType = typeid(Wrapped).name();
			std::string actualType;
			
			if (auto pointer = ptr.get()) {
				actualType = typeid(*pointer).name();
			}
			else {
				actualType = "nullptr";
			}
			
			// Throw error
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
	struct VariantConverter<juce::BorderSize<int>> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::BorderSize<int>> {};
	
	template<>
	struct VariantConverter<juce::Font> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::Font> {};
	
	template<>
	struct VariantConverter<juce::Colour> : public rxjuce::detail::ReferenceCountingVariantConverter<juce::Colour> {};
	
	template<typename ReturnType, typename... Args>
	struct VariantConverter<std::function<ReturnType(Args...)>> : public rxjuce::detail::ReferenceCountingVariantConverter<std::function<typename std::decay<ReturnType>::type(typename std::decay<Args>::type...)>> {};
	
	template<typename Enum>
	struct EnumVariantConverter
	{
		static Enum fromVar(const var &v)
		{
			return static_cast<Enum>(v.operator int());
		}
		
		static var toVar(const Enum& enumValue)
		{
			return var(enumValue);
		}
	};
	
	template<>
	struct VariantConverter<Button::ButtonState> : public EnumVariantConverter<Button::ButtonState> {};
	
	template<>
	struct VariantConverter<TextInputTarget::VirtualKeyboardType> : public EnumVariantConverter<TextInputTarget::VirtualKeyboardType> {};

	template<typename T>
	struct VariantConverter<WeakReference<T>>
	{
		static WeakReference<T> fromVar(const var &v)
		{
			if (v.isUndefined() || v.isVoid())
				return nullptr;
			else
				return rxjuce::detail::ReferenceCountingVariantConverter<WeakReference<T>>::fromVar(v);
		}
		
		static var toVar(const WeakReference<T>& weakReference)
		{
			return rxjuce::detail::ReferenceCountingVariantConverter<WeakReference<T>>::toVar(weakReference);
		}
	};
}
