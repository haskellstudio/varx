/*
  ==============================================================================

    ExampleTest.cpp
    Created: 22 Apr 2017 9:37:54am
    Author:  Martin Finke

  ==============================================================================
*/

#include "JuceHeader.h"


class ExampleTest : public UnitTest
{
public:
	ExampleTest() : UnitTest ("Example Test") {}
	void runTest() override
	{
		beginTest("Should pass");
		expect(true);
		beginTest("Should fail");
		expect(false);
	}
};

// Creating a static instance will automatically add the instance to the array
// returned by UnitTest::getAllTests(), so the test will be included when you call
// UnitTestRunner::runAllTests()
static ExampleTest test;
