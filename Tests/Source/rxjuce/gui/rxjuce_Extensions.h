/*
  ==============================================================================

    rxjuce_Extensions.h
    Created: 11 May 2017 8:15:44am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observable.h"
#include "rxjuce_Observer.h"
#include "rxjuce_Subjects.h"

RXJUCE_NAMESPACE_BEGIN

/**
	Base class for connections. Notifies when it's deallocated.
 */
class ExtensionBase
{
	const ReplaySubject _deallocated;
	
public:
	/** Notifies the ExtensionBase::deallocated with an item, and onCompleted. */
	virtual ~ExtensionBase();
	
	/**
		An Observable which emits a single item and notifies onCompleted when the ExtensionBase instance is destroyed.
	 
		If a subscription is done afterwards, it will still receive the item and the onCompleted notification.
	 
		​ **Type: undefined**
	 */
	const Observable deallocated;
	
protected:
	ExtensionBase();
};

/**
	Connects a juce::Value with a BehaviorSubject.
 
	Whenever the Value changes, the BehaviorSubject is changed, and vice versa.
 */
class ValueExtension : public ExtensionBase, private juce::Value::Listener
{
public:
	/** Creates a new instance with a given Value. The connection refers to the **`ValueSource`** of `inputValue`. */
	ValueExtension(const juce::Value& inputValue);
	
	/** The subject that's connected to the Value. This changes whenever the Value changes, and vice versa. */
	const BehaviorSubject subject;
	
private:
	juce::Value value;
	
	void valueChanged(juce::Value&) override;
};

/**
	Adds reactive extensions to a juce::Component.
 */
class ComponentExtension : public ExtensionBase, private juce::ComponentListener
{
public:
	/** Creates a new instance for a given juce::Component */
	ComponentExtension(juce::Component& parent);
	
	/** Controls the visibility of the Component, and emits an item whenever it changes.​ **Type: bool** */
	const BehaviorSubject visible;
	
private:
	void componentVisibilityChanged(juce::Component &component) override;
};

/**
	Adds reactive extensions to a juce::Button.
 */
class ButtonExtension : public ComponentExtension, private juce::Button::Listener
{
	const PublishSubject _clicked;
	const ValueExtension _toggleState;
	const PublishSubject _text;
	const PublishSubject _tooltip;
	
public:
	/** Creates a new instance for a given Button. */
	ButtonExtension(juce::Button& parent);
	
	/** Emits an item whenever the Button is clicked.​ **Type: undefined** */
	const Observable clicked;
	
	/** Controls the ButtonState.​ **Type: Button::ButtonState** */
	const BehaviorSubject buttonState;
	
	/** Controls the toggle state.​ **Type: bool** */
	const BehaviorSubject toggleState;
	
	/** Controls the button text.​ **Type: String** */
	const Observer text;
	
	/** Controls the tooltip.​ **Type: String** */
	const Observer tooltip;
	
private:
	void buttonClicked(juce::Button *) override;
	void buttonStateChanged(juce::Button *) override;
};

/**
	Adds reactive extensions to a juce::ImageComponent.
 */
class ImageComponentExtension : public ComponentExtension
{
	const PublishSubject _image;
	const PublishSubject _imagePlacement;
	
public:
	/** Creates a new instance for a given ImageComponent. */
	ImageComponentExtension(juce::ImageComponent& parent);
	
	/** Controls the displayed image.​ **Type: Image** */
	const Observer image;
	
	/** Controls the placement of the image.​ **Type: RectanglePlacement** */
	const Observer imagePlacement;
};

class LabelExtension : public ComponentExtension, private juce::Label::Listener
{
	const BehaviorSubject _discardChangesWhenHidingEditor;
	const PublishSubject _font;
	
public:
	/** Creates a new instance for a given Label. */
	LabelExtension(juce::Label& parent);
	
	/** Controls the label text. Setting a new string notifies all Label::Listeners.​ **Type: String** */
	const BehaviorSubject text;
	
	/** Controls whether the label is showing a TextEditor.​ **Type: bool** */
	const BehaviorSubject showEditor;
	
	/** Controls whether changes are discarded when hiding the TextEditor. The default is false.​ **Type: bool** */
	const Observer discardChangesWhenHidingEditor;
	
	/** Controls the Label font.​ **Type: Font** */
	const Observer font;
	
private:
	void labelTextChanged(juce::Label *) override;
	void editorShown(juce::Label *, juce::TextEditor&) override;
	void editorHidden(juce::Label *, juce::TextEditor&) override;
};

RXJUCE_NAMESPACE_END
