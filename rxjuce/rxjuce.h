#if 0
/*
BEGIN_JUCE_MODULE_DECLARATION
ID: rxjuce
vendor: martin-finke
version: 0.6.0
name: RxJUCE
description: Reactive Extensions (Rx) for JUCE.
dependencies: juce_core, juce_data_structures, juce_events, juce_graphics, juce_gui_basics
website: http://github.com/martinfinke/rxjuce
license: MIT
END_JUCE_MODULE_DECLARATION
*/
#endif

#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <exception>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>


#include "util/rxjuce_PrintFunctions.h"

namespace rxjuce {
	
typedef std::exception_ptr Error;

#include "rx/rxjuce_Disposable.h"
#include "rx/rxjuce_DisposeBag.h"
#include "rx/rxjuce_Observable.h"
#include "rx/rxjuce_Observer.h"
#include "rx/rxjuce_Scheduler.h"
#include "rx/rxjuce_Subjects.h"
	
}

#include "util/rxjuce_VariantConverters.h"

namespace rxjuce {

#include "gui/rxjuce_Extensions.h"
#include "gui/rxjuce_Reactive.h"

}
