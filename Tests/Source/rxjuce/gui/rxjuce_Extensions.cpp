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
	subject.onNext(value.getValue());
}


ComponentExtension::ComponentExtension(Component& parent)
: visible(parent.isVisible())
{
	parent.addComponentListener(this);
	
	visible.takeUntil(deallocated).subscribe([&parent](bool visible) {
		parent.setVisible(visible);
	});
}

void ComponentExtension::componentVisibilityChanged(Component& component)
{
	const bool latestItem = visible.getLatestItem();
	if (component.isVisible() != latestItem) {
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
	if (var(button->getState()) != buttonState.getLatestItem())
		buttonState.onNext(button->getState());
}

ImageComponentExtension::ImageComponentExtension(juce::ImageComponent& parent)
: rxjuce::ComponentExtension(parent),
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

RXJUCE_NAMESPACE_END
