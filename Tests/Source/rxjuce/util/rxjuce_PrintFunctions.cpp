/*
  ==============================================================================

    rxjuce_PrintFunctions.cpp
    Created: 14 May 2017 4:06:12pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_PrintFunctions.h"

namespace rxjuce {
	namespace util {
		juce::String arrayDesc(const juce::Array<juce::var>& array)
		{
			juce::StringArray strings;
			for (juce::var v : array)
				strings.add(rxjuce::util::desc(v));
			
			return rxjuce::util::desc(strings);
		}
	}
}
