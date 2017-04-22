//
//  RxJUCETestBase.h
//  RxJUCETests
//
//  Created by Martin Finke on 22.04.17.
//  Copyright © 2017 Martin Finke. All rights reserved.
//

#pragma once

#include "JuceHeader.h"

#include <functional>
#include <vector>
#include <utility>

class RxJUCETestBase : public UnitTest
{
public:
	typedef std::pair<String, std::function<void()>> TestCase;
	
	RxJUCETestBase(const String& name, const std::vector<TestCase>& testCases);
	
	void setTestToRun(int testIndex);
	
	const std::vector<TestCase> testCases;
private:
	void runTest() override;
	
	int testToRun;
};

#define RxJUCETest(__name, ...) class JUCE_JOIN_MACRO(RxJUCETest, __LINE__) : public RxJUCETestBase {\
public:\
	JUCE_JOIN_MACRO(RxJUCETest, __LINE__)() : RxJUCETestBase(#__name, {__VA_ARGS__}) {}\
};\
static JUCE_JOIN_MACRO(RxJUCETest, __LINE__) JUCE_JOIN_MACRO(test, __LINE__);
