/*
  ==============================================================================

    rxjuce_Connections.h
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

class ValueConnection : private juce::Value::Listener
{
public:
	ValueConnection(const juce::Value& inputValue);
	
	const BehaviorSubject subject;
	
private:
	juce::Value value;
	juce::Array<ScopedSubscription> subscriptions;
	
	void valueChanged(juce::Value&) override;
};

class ComponentConnection : private juce::ComponentListener
{
private:
	const PublishSubject _beingDeleted;
public:
	ComponentConnection(juce::Component& parent);
	
	const BehaviorSubject visible;
	const Observable beingDeleted;
	
private:
	void componentVisibilityChanged(juce::Component &component) override;
	void componentBeingDeleted(juce::Component &component) override;
};

class ButtonConnection : public ComponentConnection, private juce::Button::Listener
{
private:
	const PublishSubject _clicked;
	const ValueConnection _toggleState;
	const PublishSubject _text;
	const PublishSubject _tooltip;
	
public:
	ButtonConnection(juce::Button& parent);
	
	const Observable clicked;
	const BehaviorSubject buttonState;
	const BehaviorSubject toggleState;
	const Observer text;
	const Observer tooltip;
	
private:
	void buttonClicked(juce::Button *) override;
	void buttonStateChanged(juce::Button *) override;
	
	juce::Array<ScopedSubscription> subscriptions;
};

class TextButtonConnection : public ButtonConnection
{
public:
	TextButtonConnection(juce::TextButton& parent)
	: ButtonConnection(parent) {}
};

class ImageComponentConnection : public ComponentConnection
{
public:
	ImageComponentConnection(juce::ImageComponent& parent)
	: ComponentConnection(parent) {}
};

RXJUCE_NAMESPACE_END
