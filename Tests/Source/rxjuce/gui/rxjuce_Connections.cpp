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

ValueConnection::ValueConnection(const Value& inputValue)
: subject(inputValue.getValue()),
  value(inputValue)
{
	value.addListener(this);
	subscriptions.add(subject.subscribe(std::bind(&Value::setValue, value, _1)));
}

void ValueConnection::valueChanged(Value&)
{
	subject.onNext(value.getValue());
}


ComponentConnection::ComponentConnection(Component& parent)
: visible(parent.isVisible()) {}

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
  clicked(_clicked.getObservable()),
  buttonState(parent.getState()),
  toggleState(_toggleState.subject),
  text(_text.getObserver()),
  tooltip(_tooltip.getObserver())
{
	parent.addListener(this);
	
	subscriptions.add(_text.subscribe(std::bind(&Button::setButtonText, &parent, _1)));
	subscriptions.add(_tooltip.subscribe(std::bind(&Button::setTooltip, &parent, _1)));
	
	subscriptions.add(buttonState.subscribe([&parent](const var& v) {
		parent.setState(fromVar<Button::ButtonState>(v));
	}));
}

void ButtonConnection::buttonClicked(Button *)
{
	_clicked.onNext(var());
}

void ButtonConnection::buttonStateChanged(Button *button)
{
	if (var(button->getState()) != buttonState.getLatestItem())
		buttonState.onNext(button->getState());
}

ImageComponentConnection::ImageComponentConnection(juce::ImageComponent& parent)
: rxjuce::ComponentConnection(parent),
  image(_image.getObserver())
{
	subscriptions.add(_image.subscribe([&parent](const var& image) {
		parent.setImage(fromVar<Image>(image));
	}));
}

RXJUCE_NAMESPACE_END
