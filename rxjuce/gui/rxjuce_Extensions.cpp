/*
  ==============================================================================

    rxjuce_Extensions.cpp
    Created: 11 May 2017 8:15:44am
    Author:  Martin Finke

  ==============================================================================
*/

using std::placeholders::_1;

ExtensionBase::ExtensionBase()
: _deallocated(1),
  deallocated(_deallocated) {}

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
: parent(parent),
  visible(parent.isVisible())
{
	Array<Subject> colourSubjects;
	storeSubject = [colourSubjects](const Subject& subject) mutable { colourSubjects.add(subject); };
	
	parent.addComponentListener(this);
	visible.takeUntil(deallocated).subscribe(std::bind(&Component::setVisible, &parent, _1));
}

Observer ComponentExtension::colour(int colourId) const
{
	PublishSubject subject;
	
	subject.takeUntil(deallocated).subscribe([colourId, this](const var& colour) {
		this->parent.setColour(colourId, fromVar<Colour>(colour));
	});
	
	storeSubject(subject);
	return subject;
}

void ComponentExtension::componentVisibilityChanged(Component& component)
{
	if (component.isVisible() != visible.getLatestItem().operator bool()) {
		visible.onNext(component.isVisible());
	}
}


ButtonExtension::ButtonExtension(Button& parent)
: ComponentExtension(parent),
  clicked(_clicked),
  buttonState(parent.getState()),
  toggleState(parent.getToggleState()),
  text(_text),
  tooltip(_tooltip)
{
	parent.addListener(this);
	
	_text.takeUntil(deallocated).subscribe(std::bind(&Button::setButtonText, &parent, _1));
	_tooltip.takeUntil(deallocated).subscribe(std::bind(&Button::setTooltip, &parent, _1));
	
	buttonState.takeUntil(deallocated).subscribe([&parent](const var& v) {
		parent.setState(fromVar<Button::ButtonState>(v));
	});
	
	toggleState.takeUntil(deallocated).subscribe([&parent](bool toggled) {
		parent.setToggleState(toggled, sendNotificationSync);
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
	if (var(button->getToggleState()) != toggleState.getLatestItem()) {
		toggleState.onNext(button->getToggleState());
	}
}

ImageComponentExtension::ImageComponentExtension(ImageComponent& parent)
: ComponentExtension(parent),
  image(_image),
  imagePlacement(_imagePlacement)
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
  discardChangesWhenHidingEditor(_discardChangesWhenHidingEditor),
  font(_font),
  justificationType(_justificationType),
  borderSize(_borderSize),
  attachedComponent(_attachedComponent),
  attachedOnLeft(_attachedOnLeft),
  minimumHorizontalScale(_minimumHorizontalScale),
  keyboardType(_keyboardType),
  editableOnSingleClick(_editableOnSingleClick),
  editableOnDoubleClick(_editableOnDoubleClick),
  lossOfFocusDiscardsChanges(_lossOfFocusDiscardsChanges),
  textEditor(_textEditor.distinctUntilChanged())
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


SliderExtension::SliderExtension(Slider& parent, Observer getValueFromText, Observer getTextFromValue)
: ComponentExtension(parent),
  _dragging(false),
  _discardChangesWhenHidingTextBox(false),
  value(parent.getValue()),
  minimum(_minimum),
  maximum(_maximum),
  minValue(hasMultipleThumbs(parent) ? parent.getMinValue() : parent.getValue()),
  maxValue(hasMultipleThumbs(parent) ? parent.getMaxValue() : parent.getValue()),
  doubleClickReturnValue(_doubleClickReturnValue),
  interval(_interval),
  skewFactorMidPoint(_skewFactorMidPoint),
  dragging(_dragging.distinctUntilChanged()),
  thumbBeingDragged(dragging.map([&parent](bool dragging) { return (dragging ? var(parent.getThumbBeingDragged()) : var::undefined()); })),
  showTextBox(_showTextBox),
  textBoxIsEditable(_textBoxIsEditable),
  discardChangesWhenHidingTextBox(_discardChangesWhenHidingTextBox),
  getValueFromText(getValueFromText),
  getTextFromValue(getTextFromValue)
{
	parent.addListener(this);
	
	value.takeUntil(deallocated).subscribe([&parent](double value) {
		parent.setValue(value, sendNotificationSync);
	});
	
	_minimum.takeUntil(deallocated).subscribe([&parent](double minimum) {
		parent.setRange(minimum, parent.getMaximum(), parent.getInterval());
	});
	
	_maximum.takeUntil(deallocated).subscribe([&parent](double maximum) {
		parent.setRange(parent.getMinimum(), maximum, parent.getInterval());
	});
	
	minValue.skip(1).takeUntil(deallocated).subscribe([&parent](double minValue) {
		parent.setMinValue(minValue, sendNotificationSync, true);
	});
	
	maxValue.skip(1).takeUntil(deallocated).subscribe([&parent](double maxValue) {
		parent.setMaxValue(maxValue, sendNotificationSync, true);
	});
	
	_doubleClickReturnValue.takeUntil(deallocated).subscribe([&parent](var value) {
		parent.setDoubleClickReturnValue(!value.isUndefined(), value);
	});
	
	_interval.takeUntil(deallocated).subscribe([&parent](double interval) {
		parent.setRange(parent.getMinimum(), parent.getMaximum(), interval);
	});
	
	_skewFactorMidPoint.takeUntil(deallocated).subscribe(std::bind(&Slider::setSkewFactorFromMidPoint, &parent, _1));
	
	_showTextBox.withLatestFrom(_discardChangesWhenHidingTextBox).takeUntil(deallocated).subscribe([&parent](var items) {
		if (items[0])
			parent.showTextBox();
		else
			parent.hideTextBox(items[1]);
	});
	
	_textBoxIsEditable.takeUntil(deallocated).subscribe(std::bind(&Slider::setTextBoxIsEditable, &parent, _1));
}

void SliderExtension::sliderValueChanged(Slider *slider)
{
	if (slider->getValue() != value.getLatestItem().operator double()) {
		value.onNext(slider->getValue());
	}
	
	if (hasMultipleThumbs(*slider) && slider->getMinValue() != minValue.getLatestItem().operator double()) {
		minValue.onNext(slider->getMinValue());
	}
	if (hasMultipleThumbs(*slider) && slider->getMaxValue() != maxValue.getLatestItem().operator double()) {
		maxValue.onNext(slider->getMaxValue());
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

bool SliderExtension::hasMultipleThumbs(const juce::Slider& parent)
{
	switch (parent.getSliderStyle()) {
		case Slider::TwoValueHorizontal:
		case Slider::TwoValueVertical:
		case Slider::ThreeValueHorizontal:
		case Slider::ThreeValueVertical:
			return true;
			
		default:
			return false;
	}
}


