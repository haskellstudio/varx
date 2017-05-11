/*
  ==============================================================================

    rxjuce_Reactive.h
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/


#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Connections.h"

RXJUCE_NAMESPACE_BEGIN

namespace detail {
	template<typename Base, typename T>
	using Is = typename std::enable_if<std::is_base_of<Base, T>::value>::type;
	
	template<typename T>
	using IsImageComponent = typename std::enable_if<std::is_base_of<juce::ImageComponent, T>::value>::type;
	
	template<typename T>
	using IsSimpleComponent = typename std::enable_if<std::is_base_of<juce::Component, T>::value && !std::is_base_of<juce::ImageComponent, T>::value && !std::is_base_of<juce::Button, T>::value>::type;
	
	template<typename T>
	using IsSimpleButton = typename std::enable_if<std::is_base_of<juce::Button, T>::value && !std::is_base_of<juce::TextButton, T>::value>::type;
}

// If you get an error here, it means that you are trying to use an unsupported type T.
template<typename T, class Enable = void>
class Reactive;

template<typename Component>
class Reactive<Component, detail::IsSimpleComponent<Component>> : public Component
{
public:
	template<typename... Args>
	Reactive(Args&&... args)
	: Component(std::forward<Args>(args)...),
	rx(*this) {}
	
	const ComponentConnection rx;
};

template<typename ImageComponent>
class Reactive<ImageComponent, detail::IsImageComponent<ImageComponent>> : public ImageComponent
{
public:
	template<typename... Args>
	Reactive(Args&&... args)
	: ImageComponent(std::forward<Args>(args)...),
	rx(*this) {}
	
	const ImageComponentConnection rx;
};


template<typename Button>
class Reactive<Button, detail::IsSimpleButton<Button>> : public Button
{
public:
	template<typename... Args>
	Reactive(Args&&... args)
	: Button(std::forward<Args>(args)...),
	  rx(*this) {}
	
	const ButtonConnection rx;
};

template<typename Button>
class Reactive<Button, detail::Is<juce::TextButton, Button>> : public Button
{
public:
	template<typename... Args>
	Reactive(Args&&... args)
	: Button(std::forward<Args>(args)...),
	rx(*this) {}
	
	const TextButtonConnection rx;
};

template<>
class Reactive<juce::Value> : public juce::Value
{
public:
	/** Creates a new instance. Has the same behavior as the juce::Value equivalent. */
	///@{
	Reactive();
	Reactive(const Value& other);
	explicit Reactive(const juce::var& initialValue);
	///@}
	
	/** Sets a new value. This is the same as calling Observed<Value>::setValue. */
	Reactive& operator=(const juce::var& newValue);
	
	const ValueConnection rx;
	
private:
	Reactive& operator=(const Reactive&) = delete;
};

RXJUCE_NAMESPACE_END
