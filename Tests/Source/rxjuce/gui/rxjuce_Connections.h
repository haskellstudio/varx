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

class ConnectionBase
{
	const ReplaySubject _deallocated;
	
public:
	ConnectionBase();
	virtual ~ConnectionBase();
	
	const Observable deallocated;
};

class ValueConnection : public ConnectionBase, private juce::Value::Listener
{
public:
	ValueConnection(const juce::Value& inputValue);
	
	const BehaviorSubject subject;
	
private:
	juce::Value value;
	
	void valueChanged(juce::Value&) override;
};

class ComponentConnection : public ConnectionBase, private juce::ComponentListener
{
public:
	ComponentConnection(juce::Component& parent);
	
	const BehaviorSubject visible;
	
	void componentVisibilityChanged(juce::Component &component) override;
};

class ButtonConnection : public ComponentConnection, private juce::Button::Listener
{
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
	
	void buttonClicked(juce::Button *) override;
	void buttonStateChanged(juce::Button *) override;
};

class ImageComponentConnection : public ComponentConnection
{
	const PublishSubject _image;
	
public:
	ImageComponentConnection(juce::ImageComponent& parent);
	
	const Observer image;
};

RXJUCE_NAMESPACE_END
