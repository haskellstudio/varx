/*
  ==============================================================================

    varx_VariantConverters.h
    Created: 10 May 2017 9:24:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

namespace varx {
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
	
	template<typename Enum>
	struct EnumVariantConverter
	{
		static Enum fromVar(const juce::var &v)
		{
			return static_cast<Enum>(v.operator int());
		}
		
		static juce::var toVar(const Enum& enumValue)
		{
			return juce::var(enumValue);
		}
	};
}

/**
	Unwraps a type from a juce::var. It's the same as juce::VariantConverter<T>::fromVar.
 */
template<typename T>
T fromVar(const juce::var& v)
{
	return juce::VariantConverter<T>::fromVar(v);
}

/**
	Wraps a type into a juce::var. It's the same as juce::VariantConverter<T>::toVar.
 */
template<typename T>
juce::var toVar(const T& t)
{
	return juce::VariantConverter<T>::toVar(t);
}
}

namespace juce {

template<>
struct VariantConverter<varx::Observable> : public varx::detail::ReferenceCountingVariantConverter<varx::Observable> {};

template<>
struct VariantConverter<Image> : public varx::detail::ReferenceCountingVariantConverter<Image> {};

template<>
struct VariantConverter<Justification> : public varx::detail::ReferenceCountingVariantConverter<Justification> {};

template<>
struct VariantConverter<RectanglePlacement> : public varx::detail::ReferenceCountingVariantConverter<RectanglePlacement> {};

template<>
struct VariantConverter<BorderSize<int>> : public varx::detail::ReferenceCountingVariantConverter<BorderSize<int>> {};

template<>
struct VariantConverter<Font> : public varx::detail::ReferenceCountingVariantConverter<Font> {};

template<>
struct VariantConverter<Colour> : public varx::detail::ReferenceCountingVariantConverter<Colour> {};

template<typename ReturnType, typename... Args>
struct VariantConverter<std::function<ReturnType(Args...)>> : public varx::detail::ReferenceCountingVariantConverter<std::function<typename std::decay<ReturnType>::type(typename std::decay<Args>::type...)>> {};

template<>
struct VariantConverter<Button::ButtonState> : public varx::detail::EnumVariantConverter<Button::ButtonState> {};

template<>
struct VariantConverter<TextInputTarget::VirtualKeyboardType> : public varx::detail::EnumVariantConverter<TextInputTarget::VirtualKeyboardType> {};

template<typename T>
struct VariantConverter<WeakReference<T>>
{
	static WeakReference<T> fromVar(const var &v)
	{
		if (v.isUndefined() || v.isVoid())
			return nullptr;
		else
			return varx::detail::ReferenceCountingVariantConverter<WeakReference<T>>::fromVar(v);
	}
	
	static var toVar(const WeakReference<T>& weakReference)
	{
		return varx::detail::ReferenceCountingVariantConverter<WeakReference<T>>::toVar(weakReference);
	}
};

}
