/*
  ==============================================================================

    varx_Observer.h
    Created: 27 Apr 2017 7:08:56am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

class Observable;

/**
	Retrieves items. You can call onNext to notify the Observer with a new item.
 
	An Observer does **not** automatically call onCompleted when it's destroyed.
 
	@see Subject, Observable::create
 */
class Observer
{
public:
	/** Notifies the Observer with a new item. */
	void onNext(const juce::var& next) const;
	
	/** Notifies the Observer that an error has occurred. */
	void onError(Error error) const;
	
	/** Notifies the Observer that no more values will be pushed. */
	void onCompleted() const;
	
private:
	friend class Subject;
	friend class Observable;
	struct Impl;
	explicit Observer(const std::shared_ptr<Impl>& impl);
	std::shared_ptr<Impl> impl;
	
	JUCE_LEAK_DETECTOR(Observer)
};


