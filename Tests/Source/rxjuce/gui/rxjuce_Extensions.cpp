/*
  ==============================================================================

    rxjuce_Extensions.cpp
    Created: 11 May 2017 8:15:44am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Extensions.h"
#include "rxjuce_VariantConverters.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

ExtensionBase::ExtensionBase()
: _deallocated(1),
  deallocated(_deallocated.asObservable()) {}

ExtensionBase::~ExtensionBase()
{
	_deallocated.onNext(var::undefined());
	_deallocated.onCompleted();
}

ValueExtension::ValueExtension(const Value& inputValue)
: subject(inputValue.getValue()),
  value(inputValue)
{
	value.addListener(this);
	subject.takeUntil(deallocated).subscribe(std::bind(&Value::setValue, value, _1));
}

void ValueExtension::valueChanged(Value&)
{
	if (value.getValue() != subject.getLatestItem()) {
		subject.onNext(value.getValue());
	}
}


ComponentExtension::ComponentExtension(Component& parent)
: visible(parent.isVisible())
{
	parent.addComponentListener(this);
	visible.takeUntil(deallocated).subscribe(std::bind(&Component::setVisible, &parent, _1));
}

void ComponentExtension::componentVisibilityChanged(Component& component)
{
	if (component.isVisible() != visible.getLatestItem().operator bool()) {
		visible.onNext(component.isVisible());
	}
}


ButtonExtension::ButtonExtension(Button& parent)
: ComponentExtension(parent),
  _toggleState(parent.getToggleStateValue()),
  clicked(_clicked.asObservable()),
  buttonState(parent.getState()),
  toggleState(_toggleState.subject),
  text(_text.asObserver()),
  tooltip(_tooltip.asObserver())
{
	parent.addListener(this);
	
	_text.takeUntil(deallocated).subscribe(std::bind(&Button::setButtonText, &parent, _1));
	_tooltip.takeUntil(deallocated).subscribe(std::bind(&Button::setTooltip, &parent, _1));
	
	buttonState.takeUntil(deallocated).subscribe([&parent](const var& v) {
		parent.setState(fromVar<Button::ButtonState>(v));
	});
}

void ButtonExtension::buttonClicked(Button *)
{
	_clicked.onNext(var::undefined());
}

void ButtonExtension::buttonStateChanged(Button *button)
{
	if (var(button->getState()) != buttonState.getLatestItem()) {
		buttonState.onNext(button->getState());
	}
}

ImageComponentExtension::ImageComponentExtension(ImageComponent& parent)
: ComponentExtension(parent),
  image(_image.asObserver()),
  imagePlacement(_imagePlacement.asObserver())
{
	_image.takeUntil(deallocated).subscribe([&parent](const var& image) {
		parent.setImage(fromVar<Image>(image));
	});
	
	_imagePlacement.takeUntil(deallocated).subscribe([&parent](const var& imagePlacement) {
		parent.setImagePlacement(fromVar<RectanglePlacement>(imagePlacement));
	});
}

LabelExtension::LabelExtension(Label& parent)
: ComponentExtension(parent),
  _discardChangesWhenHidingEditor(false),
  text(parent.getText()),
  showEditor(parent.getCurrentTextEditor() != nullptr),
  discardChangesWhenHidingEditor(_discardChangesWhenHidingEditor.asObserver()),
  font(_font.asObserver()),
  justificationType(_justificationType.asObserver())
{
	parent.addListener(this);
	
	text.takeUntil(deallocated).subscribe(std::bind(&Label::setText, &parent, _1, sendNotificationSync));
	
	showEditor.withLatestFrom(_discardChangesWhenHidingEditor).takeUntil(deallocated).subscribe([&parent](var items) {
		if (items[0])
			parent.showEditor();
		else
			parent.hideEditor(items[1]);
	});
	
	_font.takeUntil(deallocated).subscribe([&parent](var font) {
		parent.setFont(fromVar<Font>(font));
	});
	
	_justificationType.takeUntil(deallocated).subscribe([&parent](var justificationType) {
		parent.setJustificationType(fromVar<Justification>(justificationType));
	});
}

void LabelExtension::labelTextChanged(Label *parent)
{
	if (parent->getText() != text.getLatestItem().operator String()) {
		text.onNext(parent->getText());
	}
}

void LabelExtension::editorShown(Label *parent, TextEditor&)
{
	if (!showEditor.getLatestItem()) {
		showEditor.onNext(true);
	}
}

void LabelExtension::editorHidden(Label *parent, TextEditor&)
{
	if (showEditor.getLatestItem()) {
		showEditor.onNext(false);
	}
}

RXJUCE_NAMESPACE_END
