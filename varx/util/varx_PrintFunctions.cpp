/*
  ==============================================================================

    varx_PrintFunctions.cpp
    Created: 14 May 2017 4:06:12pm
    Author:  Martin Finke

  ==============================================================================
*/

namespace util {
	juce::String arrayDesc(const juce::Array<juce::var>& array)
	{
		juce::StringArray strings;
		for (juce::var v : array)
			strings.add(varx::util::desc(v));
		
		return varx::util::desc(strings);
	}
}
