/*
  ==============================================================================

    rxjuce_Subjects_Impl.h
    Created: 14 May 2017 9:43:59am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Subjects.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx-lite.hpp"

RXJUCE_NAMESPACE_BEGIN

using namespace juce;

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
	virtual rxcpp::connectable_observable<var> multicast(const rxcpp::observable<var>& o) const = 0;
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
	
	rxcpp::connectable_observable<var> multicast(const rxcpp::observable<var>& o) const override
	{
		return o.multicast(wrapped);
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
	
	rxcpp::connectable_observable<var> multicast(const rxcpp::observable<var>& o) const override
	{
		return o.multicast(wrapped);
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
	
	rxcpp::connectable_observable<var> multicast(const rxcpp::observable<var>& o) const override
	{
		return o.multicast(wrapped);
	}
	
private:
	const rxcpp::subjects::replay<var, rxcpp::identity_one_worker> wrapped;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReplaySubjectImpl)
};

RXJUCE_NAMESPACE_END
