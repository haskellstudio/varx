/*
  ==============================================================================

    rxjuce_Scheduler_Impl.cpp
    Created: 2 May 2017 11:12:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Scheduler_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Scheduler::Impl::Impl(const Schedule& schedule)
: schedule(schedule) {}

RXJUCE_NAMESPACE_END
