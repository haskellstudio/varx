/*
  ==============================================================================

    rxjuce_Subject.cpp
    Created: 30 Apr 2017 3:01:29am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Subjects.h"

#include "rxjuce_Observable_Impl.h"
#include "rxjuce_Observer_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

class Subject::Impl
{
public:
	virtual ~Impl() {}
	virtual rxcpp::subscriber<var> getSubscriber() const = 0;
	virtual rxcpp::observable<var> asObservable() const = 0;
	virtual var getLatestItem() const
	{
		jassertfalse;
		return var::undefined();
	}
};

class BehaviorSubjectImpl : public Subject::Impl
{
public:
	BehaviorSubjectImpl(const juce::var& initial)
	: wrapped(initial) {}
	
	rxcpp::subscriber<var> getSubscriber() const override
	{
		return wrapped.get_subscriber();
	}
	
	rxcpp::observable<var> asObservable() const override
	{
		return wrapped.get_observable();
	}
	
	var getLatestItem() const override
	{
		return wrapped.get_value();
	}
	
private:
	const rxcpp::subjects::behavior<var> wrapped;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BehaviorSubjectImpl)
};

class PublishSubjectImpl : public Subject::Impl
{
public:
	PublishSubjectImpl() {}
	
	rxcpp::subscriber<var> getSubscriber() const override
	{
		return wrapped.get_subscriber();
	}
	
	rxcpp::observable<var> asObservable() const override
	{
		return wrapped.get_observable();
	}
	
private:
	const rxcpp::subjects::subject<var> wrapped;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PublishSubjectImpl)
};

class ReplaySubjectImpl : public Subject::Impl
{
public:
	ReplaySubjectImpl(size_t bufferSize)
	: wrapped(bufferSize, rxcpp::identity_immediate()) {}
	
	rxcpp::subscriber<var> getSubscriber() const override
	{
		return wrapped.get_subscriber();
	}
	
	rxcpp::observable<var> asObservable() const override
	{
		return wrapped.get_observable();
	}
	
private:
	const rxcpp::subjects::replay<var, rxcpp::identity_one_worker> wrapped;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReplaySubjectImpl)
};


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

RXJUCE_NAMESPACE_END
