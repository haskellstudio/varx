/*
  ==============================================================================

    varx_DisposeBag.h
    Created: 13 May 2017 2:58:11pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

class Disposable;

/**
	Disposes added Disposable​s when it is destroyed.
 */
class DisposeBag {
public:
	/** Creates a new, empty DisposeBag. */
	DisposeBag();
	
	/** Disposes all inserted Disposable​s in the DisposeBag. */
	~DisposeBag();
	
	/** Inserts a Disposable into the DisposeBag. The Disposable is disposed when the DisposeBag is destroyed. */
	void insert(const Disposable& disposable);
	
private:
	struct Impl;
	const std::shared_ptr<Impl> impl;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisposeBag)
};


