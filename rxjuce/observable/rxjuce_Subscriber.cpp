//
//  rxjuce_Subscriber.cpp
//  RxJUCE
//
//  Created by Martin Finke on 26.04.17.
//
//

Subscriber::Subscriber(const std::function<void(const juce::var&)>& onNext)
: _onNext(onNext)
{}

void Subscriber::onNext(const juce::var& next) const
{
	_onNext(next);
}
