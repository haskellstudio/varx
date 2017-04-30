/*
  ==============================================================================

    rxjuce_TestUtilities.h
    Created: 27 Apr 2017 7:55:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_PrintFunctions.h"
#include "rxjuce_Subscription.h"

#include "catch.hpp"

using namespace juce;
using namespace rxjuce;

#define CONTEXT( desc )     SECTION( std::string(" Context: ") + desc, "" )
#define IT( desc )			SECTION( std::string("       It ") + desc, "" )

/** Subscribes to an Observable and collects all emitted items into a given Array. */
#define RxJUCECollectItems(__observable, __arrayName) const ScopedSubscription JUCE_JOIN_MACRO(__arrayName, JUCE_JOIN_MACRO(Subscription_, __LINE__))((__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); }))

/** REQUIREs that a given Array is equal to the list of passed items. */
#define RxJUCERequireResults(__arrayName, ...) REQUIRE(__arrayName == Array<var>({__VA_ARGS__}))


/** Runs the JUCE dispatch loop for a given time, to process async callbacks. */
inline void RxJUCERunDispatchLoop(int millisecondsToRunFor = 0)
{
	MessageManager::getInstance()->runDispatchLoopUntil(millisecondsToRunFor);
}
