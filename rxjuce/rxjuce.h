#if 0
/*
BEGIN_JUCE_MODULE_DECLARATION
ID: rxjuce
vendor: martin-finke
version: 1.0.0
name: Reactive Extensions (Rx) for JUCE.
description: Combines Rx Observable sequences with JUCE types.
dependencies: juce_core, juce_data_structures, juce_events
website: http://www.martin-finke.de
license: MIT
END_JUCE_MODULE_DECLARATION
*/
#endif

#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

#include "observable/rxjuce_Observable.h"
#include "observable/rxjuce_Subscription.h"
#include "value/rxjuce_ValueSource.h"
