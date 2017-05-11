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
	virtual rxcpp::observable<var> getObservable() const = 0;
	virtual var getLatestItem() const = 0;
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
	
	rxcpp::observable<var> getObservable() const override
	{
		return wrapped.get_observable();
	}
	
	var getLatestItem() const override
	{
		return wrapped.get_value();
	}
	
private:
	const rxcpp::subjects::behavior<var> wrapped;
};

class PublishSubjectImpl : public Subject::Impl
{
public:
	rxcpp::subscriber<var> getSubscriber() const override
	{
		return wrapped.get_subscriber();
	}
	
	rxcpp::observable<var> getObservable() const override
	{
		return wrapped.get_observable();
	}
	
	var getLatestItem() const override
	{
		jassertfalse;
		return var::undefined();
	}
	
private:
	const rxcpp::subjects::subject<var> wrapped;
};


Subject::Subject(const std::shared_ptr<Impl>& impl)
: Observer(std::make_shared<Observer::Impl>(impl->getSubscriber())),
  Observable(std::make_shared<Observable::Impl>(impl->getObservable())),
  impl(impl) {}

Observable Subject::getObservable() const
{
	return *this;
}

Observer Subject::getObserver() const
{
	return *this;
}

BehaviorSubject::BehaviorSubject(const juce::var& initial)
: Subject(std::make_shared<BehaviorSubjectImpl>(initial)) {}

var BehaviorSubject::getLatestItem() const
{
	return Subject::impl->getLatestItem();
}

PublishSubject::PublishSubject()
: Subject(std::make_shared<PublishSubjectImpl>()) {}

RXJUCE_NAMESPACE_END
