/*
  ==============================================================================

    varx_PrintFunctions.h
    Created: 27 Apr 2017 7:49:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#define VARX_DEFINE_PRINT_FUNCTION(__type, __body) namespace varx {\
namespace util {\
	inline juce::String desc(const __type& value) __body\
}\
}\
namespace juce {\
	inline std::ostream& operator<<(std::ostream& os, const __type& value) {\
		os << varx::util::desc(value);\
		return os;\
	}\
}

namespace varx {
	namespace util {
		juce::String arrayDesc(const juce::Array<juce::var>& array);
	}
}

VARX_DEFINE_PRINT_FUNCTION(juce::var, {
	if (value.isBool())
		return (value ? "true" : "false");
	else if (value.isVoid())
		return "void";
	else if (value.isUndefined())
		return "undefined";
	else if (value.isString())
		return "\"" + value.toString() + "\"";
	else if (value.isArray())
		return varx::util::arrayDesc(*value.getArray());
	else
		return value.toString();
})

VARX_DEFINE_PRINT_FUNCTION(juce::StringArray, {
	return "{" + value.joinIntoString(", ") + "}";
})

VARX_DEFINE_PRINT_FUNCTION(juce::Array<juce::var>, {
	return varx::util::arrayDesc(value);
})

#undef VARX_DEFINE_PRINT_FUNCTION
