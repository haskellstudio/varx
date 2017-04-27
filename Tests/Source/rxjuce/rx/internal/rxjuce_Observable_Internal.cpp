/*
  ==============================================================================

    rxjuce_Observable_Internal.cpp
    Created: 27 Apr 2017 7:11:30am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Observable_Internal.h"

#include "rxjuce_Subscriber.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Observable::Internal::Ptr Observable::Internal::fromRxCpp(const rxcpp::observable<var>& o)
{
	return Ptr(new Internal(o));
}

Observable::Internal::Internal(const rxcpp::observable<var>& o)
:	o(o)
{}

RXJUCE_NAMESPACE_END
