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
	/** Notifies the ExtensionBase::deallocated Observable with an item, and onCompleted. */
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

/**
	Adds reactive extensions to a juce::Label.
 */
class LabelExtension : public ComponentExtension, private juce::Label::Listener
{
	const BehaviorSubject _discardChangesWhenHidingEditor;
	const PublishSubject _font;
	const PublishSubject _justificationType;
	const PublishSubject _borderSize;
	const PublishSubject _attachedComponent;
	const PublishSubject _attachedOnLeft;
	const PublishSubject _minimumHorizontalScale;
	const PublishSubject _keyboardType;
	const PublishSubject _editableOnSingleClick;
	const PublishSubject _editableOnDoubleClick;
	const PublishSubject _lossOfFocusDiscardsChanges;
	const BehaviorSubject _textEditor;
	
public:
	/** Creates a new instance for a given Label. */
	LabelExtension(juce::Label& parent);
	
	/** Controls the Label's text. Setting a new string notifies all Label::Listeners.​ **Type: String** */
	const BehaviorSubject text;
	
	/** Controls whether the Label is showing a TextEditor.​ **Type: bool** */
	const BehaviorSubject showEditor;
	
	/** Controls whether changes are discarded when hiding the TextEditor. The default is false.​ **Type: bool** */
	const Observer discardChangesWhenHidingEditor;
	
	/** Controls the Label's font.​ **Type: Font** */
	const Observer font;
	
	/** Controls the Label's justification.​ **Type: Justification** */
	const Observer justificationType;
	
	/** Controls the Label's border size.​ **Type: BorderSize<int>** */
	const Observer borderSize;
	
	/** Attaches the Label to another Component.​ **Type: WeakReference<Component>, or `var::undefined()` if no Component should be attached.** */
	const Observer attachedComponent;
	
	/** Controls whether the attachedComponent should be attached on the left.​ **Type: bool** */
	const Observer attachedOnLeft;
	
	/** Controls  the minimum amount that the Label font can be squashed horizontally before it starts using ellipsis.​ **Type: float** */
	const Observer minimumHorizontalScale;
	
	/** Controls the keyboard type to use in the TextEditor. If the editor is currently open, the type is changed for the open editor.​ **Type: TextInputTarget::VirtualKeyboardType** */
	const Observer keyboardType;
	
	/** Controls whether clicking the Label opens a TextEditor.​ **Type: bool** */
	const Observer editableOnSingleClick;
	
	/** Controls whether double-clicking the Label opens a TextEditor.​ **Type: bool** */
	const Observer editableOnDoubleClick;
	
	/** Controls whether unfocussing the TextEditor discards changes.​ **Type: bool** */
	const Observer lossOfFocusDiscardsChanges;
	
	/** The currently visible TextEditor.​ **Type: WeakReference<Component>, or `var::undefined()` if no TextEditor is showing.** */
	const Observable textEditor;
	
private:
	void labelTextChanged(juce::Label *) override;
	void editorShown(juce::Label *, juce::TextEditor&) override;
	void editorHidden(juce::Label *, juce::TextEditor&) override;
	
	static juce::var getTextEditor(juce::Label& label);
};



/**
	Adds reactive extensions to a juce::Slider.
 */
class SliderExtension : public ComponentExtension, private juce::Slider::Listener
{
	BehaviorSubject _dragging;
	
public:
	/** Creates a new instance for a given Slider. */
	SliderExtension(juce::Slider& parent);
	
	/** Controls the current Slider value.​ **Type: double** */
	const BehaviorSubject value;
	
	/** Whether the Slider is currently being dragged.​ **Type: bool** */
	const Observable dragging;
	
private:
	void sliderValueChanged(juce::Slider *slider) override;
	void sliderDragStarted(juce::Slider *) override;
	void sliderDragEnded(juce::Slider *) override;
};

RXJUCE_NAMESPACE_END
