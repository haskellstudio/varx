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

using namespace juce;
using namespace rxjuce;

#define CONTEXT( desc )     SECTION( std::string(" Context: ") + desc, "" )
#define IT( desc )			SECTION( std::string("       It ") + desc, "" )

#define RxJUCECollectItems(__observable, __arrayName) const RAIISubscription JUCE_JOIN_MACRO(__arrayName, JUCE_JOIN_MACRO(Subscription_, __LINE__))((__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); }))

#define RxJUCERequireResults(__arrayName, ...) REQUIRE(__arrayName == Array<var>({__VA_ARGS__}))


inline void RxJUCERunDispatchLoop(int millisecondsToRunFor = 0)
{
	MessageManager::getInstance()->runDispatchLoopUntil(millisecondsToRunFor);
}
