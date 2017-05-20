/*
  ==============================================================================

    rxjuce_Reactive.h
    Created: 27 Apr 2017 8:51:03am
    Author:  Martin Finke

  ==============================================================================
*/


#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Extensions.h"

RXJUCE_NAMESPACE_BEGIN

namespace detail {
	template<typename Base, typename T>
	using Is = typename std::enable_if<std::is_base_of<Base, T>::value>::type;
	
	template<typename T>
	using IsImageComponent = typename std::enable_if<std::is_base_of<juce::ImageComponent, T>::value>::type;
	
	template<typename T>
	using IsSimpleComponent = typename std::enable_if<std::is_base_of<juce::Component, T>::value && !std::is_base_of<juce::ImageComponent, T>::value && !std::is_base_of<juce::Button, T>::value && !std::is_base_of<juce::Label, T>::value && !std::is_base_of<juce::Slider, T>::value>::type;
	
	template<typename T>
	using IsButton = typename std::enable_if<std::is_base_of<juce::Button, T>::value>::type;
	
	template<typename T>
	using IsLabel = typename std::enable_if<std::is_base_of<juce::Label, T>::value>::type;
	
	template<typename T>
	using IsSlider = typename std::enable_if<std::is_base_of<juce::Slider, T>::value>::type;
}

// If you get an error here, it means that you are trying to use an unsupported type T.
template<typename T, typename Enable = void>
class Reactive;

/**
	Adds reactive extensions to a juce::Value.
 
	Instead of creating a juce::Value, create an instance of this as follows:
 
		Reactive<Value> myValue;
 
	It inherits from juce::Value, so you can use it as a drop-in replacement. And you can access `myValue.rx.subject` to subscribe to changes, etc.:
 
		myValue.rx.subject.map(...).filter(...).subscribe(...);
 
	And you can bind this value to some Observable, to change the value whenever the Observable emits an item:
	
		myValue.rx.subject.bindTo(someObservable);
 */
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
	
	/** The reactive extension object. */
	const ValueExtension rx;
	
private:
	Reactive& operator=(const Reactive&) = delete;
};

/**
	Adds reactive extensions to a juce::Component (or subclass).
 */
template<typename Component>
class Reactive<Component, detail::IsSimpleComponent<Component>> : public Component
{
public:
	/** Creates a new instance. @see juce::Component::Component. */
	template<typename... Args>
	Reactive(Args&&... args)
	: Component(std::forward<Args>(args)...),
	rx(*this) {}
	
	/** The reactive extension object. */
	const ComponentExtension rx;
};

/**
	Adds reactive extensions to a juce::ImageComponent (or subclass).
 */
template<typename ImageComponent>
class Reactive<ImageComponent, detail::IsImageComponent<ImageComponent>> : public ImageComponent
{
public:
	/** Creates a new instance. @see juce::ImageComponent::ImageComponent. */
	template<typename... Args>
	Reactive(Args&&... args)
	: ImageComponent(std::forward<Args>(args)...),
	rx(*this) {}
	
	/** The reactive extension object. */
	const ImageComponentExtension rx;
};

/**
	Adds reactive extensions to a juce::Button (or subclass).
 */
template<typename Button>
class Reactive<Button, detail::IsButton<Button>> : public Button
{
public:
	/** Creates a new instance. @see juce::Button::Button. */
	template<typename... Args>
	Reactive(Args&&... args)
	: Button(std::forward<Args>(args)...),
	  rx(*this) {}
	
	/** The reactive extension object. */
	const ButtonExtension rx;
};

/**
	Adds reactive extensions to a juce::Label (or subclass).
 */
template<typename Label>
class Reactive<Label, detail::IsLabel<Label>> : public Label
{
public:
	/** Creates a new instance. @see juce::Label::Label. */
	template<typename... Args>
	Reactive(Args&&... args)
	: Label(std::forward<Args>(args)...),
	  rx(*this) {}
	
	/** The reactive extension object. */
	const LabelExtension rx;
};

/**
	Adds reactive extensions to a juce::Slider (or subclass).
 */
template<typename Slider>
class Reactive<Slider, detail::IsSlider<Slider>> : public Slider
{
public:
	/** Creates a new instance. @see juce::Slider::Slider. */
	template<typename... Args>
	Reactive(Args&&... args)
	: Slider(std::forward<Args>(args)...),
	rx(*this) {}
	
	/** The reactive extension object. */
	const SliderExtension rx;
};

RXJUCE_NAMESPACE_END
