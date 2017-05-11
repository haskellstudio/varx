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

class ComponentConnection
{
public:
	ComponentConnection(juce::Component& parent) {}
};

class ButtonConnection : public ComponentConnection, private juce::Button::Listener
{
private:
	const PublishSubject _clicked;
	
public:
	ButtonConnection(juce::Button& parent);
	
	const Observable clicked;
	const BehaviorSubject buttonState;
	
private:
	void buttonClicked(juce::Button *) override;
	void buttonStateChanged(juce::Button *) override;
	
	juce::Array<ScopedSubscription> subscriptions;
};

class DrawableButtonConnection : public ButtonConnection
{
public:
	DrawableButtonConnection(juce::DrawableButton& parent);
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

class ValueConnection
{
public:
	ValueConnection(const juce::Value& parent);
	
	const Observable observable;
};

RXJUCE_NAMESPACE_END
