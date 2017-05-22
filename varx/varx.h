#if 0
/*
BEGIN_JUCE_MODULE_DECLARATION
ID: varx
vendor: martin-finke
version: 0.6.1
name: varx
description: Reactive Extensions (Rx) for JUCE.
dependencies: juce_core, juce_data_structures, juce_events, juce_graphics, juce_gui_basics
website: http://github.com/martinfinke/varx
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


#include "util/varx_PrintFunctions.h"

namespace varx {
	
typedef std::exception_ptr Error;

#include "rx/varx_Disposable.h"
#include "rx/varx_DisposeBag.h"
#include "rx/varx_Observable.h"
#include "rx/varx_Observer.h"
#include "rx/varx_Scheduler.h"
#include "rx/varx_Subjects.h"
	
}

#include "util/varx_VariantConverters.h"

namespace varx {

#include "gui/varx_Extensions.h"
#include "gui/varx_Reactive.h"

}
