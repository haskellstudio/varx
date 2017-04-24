#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "JuceHeader.h"

int main(int argc, char* argv[])
{
	// global setup...
	juce::MessageManager::getInstance();
	
	int result = Catch::Session().run( argc, argv );
	
	// global clean-up...
	
	juce::MessageManager::deleteInstance();
	
	return ( result < 0xff ? result : 0xff );
}
