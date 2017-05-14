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
	
	const String getApplicationName() override       { return ProjectInfo::projectName; }
	const String getApplicationVersion() override    { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override       { return true; }
};

START_JUCE_APPLICATION(TestRunnerApplication)
