/*
  ==============================================================================

    rxjuce_TestUtilities.h
    Created: 27 Apr 2017 7:55:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Observable.h"
#include "rxjuce_Subscriber.h"
#include "rxjuce_ObservedButton.h"
#include "rxjuce_PrintFunctions.h"
#include "catch.hpp"

#define RxJUCECollectResult(__observable, __resultName) var __resultName; (__observable).subscribe([&__resultName](var v){ __resultName = v; })

#define RxJUCECollectResults(__observable, __arrayName) Array<var> __arrayName; (__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); })

using namespace juce;
using namespace rxjuce;
