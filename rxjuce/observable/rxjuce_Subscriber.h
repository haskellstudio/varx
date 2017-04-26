//
//  rxjuce_Subscriber.hpp
//  RxJUCE
//
//  Created by Martin Finke on 26.04.17.
//
//

#pragma once

class Subscriber
{
public:
	void onNext(const juce::var& next) const;
	
private:
	friend class Observable;
	Subscriber(const std::function<void(const juce::var&)>& onNext);
	
	std::function<void(const juce::var&)> _onNext;
};
