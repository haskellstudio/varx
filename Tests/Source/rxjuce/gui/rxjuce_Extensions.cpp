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
  _textEditor(getTextEditor(parent)),
  text(parent.getText()),
  showEditor(parent.getCurrentTextEditor() != nullptr),
  discardChangesWhenHidingEditor(_discardChangesWhenHidingEditor.asObserver()),
  font(_font.asObserver()),
  justificationType(_justificationType.asObserver()),
  borderSize(_borderSize.asObserver()),
  attachedComponent(_attachedComponent.asObserver()),
  attachedOnLeft(_attachedOnLeft.asObserver()),
  minimumHorizontalScale(_minimumHorizontalScale.asObserver()),
  keyboardType(_keyboardType.asObserver()),
  editableOnSingleClick(_editableOnSingleClick.asObserver()),
  editableOnDoubleClick(_editableOnDoubleClick.asObserver()),
  lossOfFocusDiscardsChanges(_lossOfFocusDiscardsChanges.asObserver()),
  textEditor(_textEditor.asObservable().distinctUntilChanged())
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
	
	_borderSize.takeUntil(deallocated).subscribe([&parent](var borderSize) {
		parent.setBorderSize(fromVar<BorderSize<int>>(borderSize));
	});
	
	_attachedComponent.takeUntil(deallocated).subscribe([&parent](var component) {
		parent.attachToComponent(fromVar<WeakReference<Component>>(component), parent.isAttachedOnLeft());
	});
	
	_attachedOnLeft.takeUntil(deallocated).subscribe([&parent](bool attachedOnLeft) {
		parent.attachToComponent(parent.getAttachedComponent(), attachedOnLeft);
	});
	
	_minimumHorizontalScale.takeUntil(deallocated).subscribe(std::bind(&Label::setMinimumHorizontalScale, &parent, _1));
	
	_keyboardType.takeUntil(deallocated).subscribe([&parent](var v) {
		const auto keyboardType = fromVar<TextInputTarget::VirtualKeyboardType>(v);
		parent.setKeyboardType(keyboardType);
		
		if (auto editor = parent.getCurrentTextEditor()) {
			editor->setKeyboardType(keyboardType);
		}
	});
	
	_editableOnSingleClick.takeUntil(deallocated).subscribe([&parent](bool editable) {
		parent.setEditable(editable, parent.isEditableOnDoubleClick(), parent.doesLossOfFocusDiscardChanges());
	});
	
	_editableOnDoubleClick.takeUntil(deallocated).subscribe([&parent](bool editable) {
		parent.setEditable(parent.isEditableOnSingleClick(), editable, parent.doesLossOfFocusDiscardChanges());
	});
	
	_lossOfFocusDiscardsChanges.takeUntil(deallocated).subscribe([&parent](bool lossOfFocusDiscardsChanges) {
		parent.setEditable(parent.isEditableOnSingleClick(), parent.isEditableOnDoubleClick(), lossOfFocusDiscardsChanges);
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
	
	_textEditor.onNext(getTextEditor(*parent));
}

void LabelExtension::editorHidden(Label *parent, TextEditor&)
{
	if (showEditor.getLatestItem()) {
		showEditor.onNext(false);
	}
	
	_textEditor.onNext(getTextEditor(*parent));
}

var LabelExtension::getTextEditor(Label& label)
{
	if (auto editor = label.getCurrentTextEditor())
		return toVar(WeakReference<Component>(editor));
	else
		return var::undefined();
}


SliderExtension::SliderExtension(Slider& parent)
: ComponentExtension(parent),
  _dragging(false),
  value(parent.getValue()),
  dragging(_dragging.asObservable().distinctUntilChanged())
{
	parent.addListener(this);
	
	value.takeUntil(deallocated).subscribe([&parent](double value) {
		parent.setValue(value, sendNotificationSync);
	});
}

void SliderExtension::sliderValueChanged(Slider *slider)
{
	if (slider->getValue() != value.getLatestItem().operator double()) {
		value.onNext(slider->getValue());
	}
}

void SliderExtension::sliderDragStarted(Slider *)
{
	_dragging.onNext(true);
}

void SliderExtension::sliderDragEnded(Slider *)
{
	_dragging.onNext(false);
}

RXJUCE_NAMESPACE_END
