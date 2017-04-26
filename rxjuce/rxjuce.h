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

//==============================================================================
/** Config: RXJUCE_ENABLE_LONG_SIGNATURES
 If this is set to 0, only overloads with short signatures are added for Rx Observable operators. This can reduce compile time.
 */
#ifndef RXJUCE_ENABLE_LONG_SIGNATURES
#define RXJUCE_ENABLE_LONG_SIGNATURES 1
#endif

#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

namespace rxjuce {

#include "observable/rxjuce_Observable.h"
#include "observable/rxjuce_Subscription.h"

}
