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
	ExampleTest() : UnitTest("Example Test") {}
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

class RxJUCETestCaseBase : public UnitTest {
protected:
	RxJUCETestCaseBase(const String& testName) : UnitTest(testName) {}
};





#define RxJUCETestCase(__testName, __body) class JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName);\
class JUCE_JOIN_MACRO(RxJUCETestCase_, __testName) : public RxJUCETestCaseBase {\
public:\
	JUCE_JOIN_MACRO(RxJUCETestCase_, __testName)() : RxJUCETestCaseBase(#__testName) {}\
protected:\
	static HashMap<String, void(JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName)::*)()> testMethods;\
	friend class RegisterTestMethod;\
	class RegisterTestMethod {\
	public:\
		RegisterTestMethod(const String& name, void(JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName)::*testMethod)())\
		{\
			JUCE_JOIN_MACRO(RxJUCETestCase_, __testName)::testMethods.set(name, testMethod);\
		}\
	};\
};\
class JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName) : public JUCE_JOIN_MACRO(RxJUCETestCase_, __testName) __body;\
HashMap<String, void(JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName)::*)()> JUCE_JOIN_MACRO(RxJUCETestCase_, __testName)::testMethods;\
class JUCE_JOIN_MACRO(RxJUCETestCaseRunTests_, __testName) : public JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName) {\
	void runTest() override\
	{\
		for (HashMap<String, void(JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __testName)::*)()>::Iterator it(testMethods); it.next();) {\
			beginTest(it.getKey());\
			(this->*it.getValue())();\
		}\
	}\
};\
static JUCE_JOIN_MACRO(RxJUCETestCaseRunTests_, __testName) JUCE_JOIN_MACRO(rxJUCETestCase_, __testName)\


#define RxJUCETest(__className, __name, __body) void JUCE_JOIN_MACRO(testMethod_, __LINE__)() __body;\
const RegisterTestMethod JUCE_JOIN_MACRO(registerTestMethod_, __LINE__) = RegisterTestMethod((__name), &JUCE_JOIN_MACRO(RxJUCETestCaseMethods_, __className)::JUCE_JOIN_MACRO(testMethod_, __LINE__))



RxJUCETestCase(ExampleTest, {
	RxJUCETest(ExampleTest, "It should output audio", {
		bool thereWasOutput = false;
		expect(thereWasOutput, "There should have been audio output.");
	});

	RxJUCETest(ExampleTest, "It should output audio", {
		bool thereWasOutput = false;
		expect(thereWasOutput, "There should have been audio output.");
	});
});

RxJUCETestCase(AnotherExample, {
	RxJUCETest(AnotherExample, "It should output audio", {
		bool thereWasOutput = false;
		expect(thereWasOutput, "There should have been audio output.");
	});
	
	RxJUCETest(AnotherExample, "It should output audio", {
		bool thereWasOutput = false;
		expect(thereWasOutput, "There should have been audio output.");
	});
});
