/*
  ==============================================================================

    rxjuce_Scheduling.h
    Created: 2 May 2017 11:12:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"

#include "rxjuce_Observable.h"

#include "../../RxCpp/Rx/v2/src/rxcpp/rx.hpp"

RXJUCE_NAMESPACE_BEGIN

namespace scheduling {
	rxcpp::observe_on_one_worker juceMessageThread();
	rxcpp::synchronize_in_one_worker rxcppEventLoop();
	rxcpp::synchronize_in_one_worker newThread();
}

RXJUCE_NAMESPACE_END
