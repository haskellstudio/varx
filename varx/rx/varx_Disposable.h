/*
  ==============================================================================

    varx_Disposable.h
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

class DisposeBag;

/**
	Manages the lifetime of a disposable to an Observable.
 
	@see Observable::subscribe
 */
class Disposable
{
public:
	
	///@{
	/**
		Transfers ownership from an existing Disposable.
	 */
	Disposable(Disposable&&) = default;
	Disposable& operator=(Disposable&&) = default;
	///@}
	
	/**
		Unsubscribes from the Observable.
	 */
	void dispose() const;
	
	/**
		Inserts the Disposable into a given DisposeBag.
		
		The Disposable is disposed when the DisposeBag is destroyed.
		
		@see DisposeBag::insert
	 */
	void disposedBy(DisposeBag& disposeBag);
	
private:
	struct Impl;
	const std::shared_ptr<Impl> impl;
	
	friend class Observable;
	friend class DisposeBag;
	explicit Disposable(const std::shared_ptr<Impl>&);
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Disposable)
};


