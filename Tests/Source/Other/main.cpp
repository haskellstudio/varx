#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "JuceHeader.h"

class TestRunnerApplication : public JUCEApplication
{
public:
	void initialise (const String& commandLine) override
	{
		
		Catch::ConfigData config;
		config.shouldDebugBreak = true;
//		config.testsOrTags = {"ComponentExtension"};
		
		Catch::Session session;
		session.useConfigData(config);
		session.run();
		
		quit();
	}
	
	void shutdown() override
	{}
	
	const String getApplicationName() override
	{
		return "RxJUCE-Tests";
	}
	
	const String getApplicationVersion() override
	{
		return "1.0";
	}
};

START_JUCE_APPLICATION(TestRunnerApplication)
