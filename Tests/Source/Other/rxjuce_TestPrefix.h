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

#define RxJUCECollectResults(__observable, __arrayName) const RAIISubscription JUCE_JOIN_MACRO(__arrayName, JUCE_JOIN_MACRO(Subscription_, __LINE__))((__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); }))

#define RxJUCERequireResults(__arrayName, ...) REQUIRE(__arrayName == Array<var>({__VA_ARGS__}))

inline void RxJUCERunDispatchLoop(int millisecondsToRunFor = 0)
{
	juce::MessageManager::getInstance()->runDispatchLoopUntil(millisecondsToRunFor);
}

using namespace juce;
using namespace rxjuce;
