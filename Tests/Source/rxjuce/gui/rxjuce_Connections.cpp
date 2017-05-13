/*
  ==============================================================================

    rxjuce_Connections.cpp
    Created: 11 May 2017 8:15:44am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Connections.h"
#include "rxjuce_VariantConverters.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

ConnectionBase::ConnectionBase()
: _deallocated(1),
  deallocated(_deallocated.asObservable()) {}

ConnectionBase::~ConnectionBase()
{
	_deallocated.onNext(var::undefined());
	_deallocated.onCompleted();
}

ValueConnection::ValueConnection(const Value& inputValue)
: subject(inputValue.getValue()),
  value(inputValue)
{
	value.addListener(this);
	subject.takeUntil(deallocated).subscribe(std::bind(&Value::setValue, value, _1));
}

void ValueConnection::valueChanged(Value&)
{
	subject.onNext(value.getValue());
}


ComponentConnection::ComponentConnection(Component& parent)
: visible(parent.isVisible())
{
	parent.addComponentListener(this);
	
	visible.takeUntil(deallocated).subscribe([&parent](bool visible) {
		parent.setVisible(visible);
	});
}

void ComponentConnection::componentVisibilityChanged(Component& component)
{
	const bool latestItem = visible.getLatestItem();
	if (component.isVisible() != latestItem) {
		visible.onNext(component.isVisible());
	}
}


ButtonConnection::ButtonConnection(Button& parent)
: ComponentConnection(parent),
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

void ButtonConnection::buttonClicked(Button *)
{
	_clicked.onNext(var::undefined());
}

void ButtonConnection::buttonStateChanged(Button *button)
{
	if (var(button->getState()) != buttonState.getLatestItem())
		buttonState.onNext(button->getState());
}

ImageComponentConnection::ImageComponentConnection(juce::ImageComponent& parent)
: rxjuce::ComponentConnection(parent),
  image(_image.asObserver())
{
	_image.takeUntil(deallocated).subscribe([&parent](const var& image) {
		parent.setImage(fromVar<Image>(image));
	});
}

RXJUCE_NAMESPACE_END
