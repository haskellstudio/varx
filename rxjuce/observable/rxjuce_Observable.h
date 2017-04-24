//
//  rxjuce_Observable.h
//  RxJUCE
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Subscription.h"

#include <functional>
#include <memory>

namespace rxjuce {
	class Observable
	{
	public:
		Observable(const juce::Value& value);
		
		Subscription subscribe(const std::function<void(juce::var)>& f);
		
		static Observable just(juce::var value);
		
		Observable map(const std::function<juce::var(juce::var)>& transform);
		
	private:
		class Internal;
		typedef std::shared_ptr<Internal> Internal_Ptr;
		
		Observable(const Internal_Ptr& internal);
		static Internal_Ptr InternalFromValue(const juce::Value& value);
		
		const Internal_Ptr internal;
	};
}
