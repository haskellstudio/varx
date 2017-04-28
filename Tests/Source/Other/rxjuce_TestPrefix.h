/*
  ==============================================================================

    rxjuce_TestUtilities.h
    Created: 27 Apr 2017 7:55:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Observable.h"
#include "rxjuce_Subscription.h"
#include "rxjuce_Observed.h"
#include "rxjuce_PrintFunctions.h"
#include "catch.hpp"

#define RxJUCECollectResult(__observable, __resultName) var __resultName; (__observable).subscribe([&__resultName](var v){ __resultName = v; })

#define RxJUCECollectResults(__observable, __arrayName) Array<var> __arrayName; const RAIISubscription __arrayName##Subscription((__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); }))

#define RxJUCERequireResults(__arrayName, ...) REQUIRE(__arrayName == Array<var>({__VA_ARGS__}))

inline void RxJUCERunDispatchLoop()
{
	juce::MessageManager::getInstance()->runDispatchLoopUntil(0);
}

using namespace juce;
using namespace rxjuce;
