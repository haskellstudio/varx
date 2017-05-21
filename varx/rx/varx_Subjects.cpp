/*
  ==============================================================================

    varx_Subject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

Subject::Subject(const std::shared_ptr<Impl>& impl)
: Observer(std::make_shared<Observer::Impl>(impl->getSubscriber())),
  Observable(std::make_shared<Observable::Impl>(impl->asObservable())),
  impl(impl) {}

Observable Subject::asObservable() const
{
	return *this;
}

Observer Subject::asObserver() const
{
	return *this;
}

BehaviorSubject::BehaviorSubject(const juce::var& initial)
: Subject(std::make_shared<BehaviorSubjectImpl>(initial)) {}

var BehaviorSubject::getLatestItem() const
{
	return impl->getLatestItem();
}

PublishSubject::PublishSubject()
: Subject(std::make_shared<PublishSubjectImpl>()) {}

ReplaySubject::ReplaySubject(size_t bufferSize)
: Subject(std::make_shared<ReplaySubjectImpl>(bufferSize)) {}

const size_t ReplaySubject::MaxBufferSize = std::numeric_limits<size_t>::max();


