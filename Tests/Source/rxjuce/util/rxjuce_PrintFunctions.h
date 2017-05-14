/*
  ==============================================================================

    rxjuce_PrintFunctions.h
    Created: 27 Apr 2017 7:49:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "rxjuce_Prefix.h"
#include <iostream>

#define RXJUCE_DEFINE_PRINT_FUNCTION(__type, __body) namespace rxjuce {\
namespace util {\
	inline juce::String desc(const __type& value) __body\
}\
}\
namespace juce {\
	inline std::ostream& operator<<(std::ostream& os, const __type& value) {\
		os << rxjuce::util::desc(value);\
		return os;\
	}\
}

namespace rxjuce {
	namespace util {
		juce::String arrayDesc(const juce::Array<juce::var>& array);
	}
}

RXJUCE_DEFINE_PRINT_FUNCTION(juce::var, {
	if (value.isBool())
		return (value ? "true" : "false");
	else if (value.isVoid())
		return "void";
	else if (value.isUndefined())
		return "undefined";
	else if (value.isString())
		return "\"" + value.toString() + "\"";
	else if (value.isArray())
		return rxjuce::util::arrayDesc(*value.getArray());
	else
		return value.toString();
})

RXJUCE_DEFINE_PRINT_FUNCTION(juce::StringArray, {
	return "{" + value.joinIntoString(", ") + "}";
})

RXJUCE_DEFINE_PRINT_FUNCTION(juce::Array<juce::var>, {
	return rxjuce::util::arrayDesc(value);
})

#undef RXJUCE_DEFINE_PRINT_FUNCTION
