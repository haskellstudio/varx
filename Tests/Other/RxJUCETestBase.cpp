//
//  RxJUCETestBase.cpp
//  RxJUCETests
//
//  Created by Martin Finke on 22.04.17.
//  Copyright © 2017 Martin Finke. All rights reserved.
//

#include "RxJUCETestBase.h"

RxJUCETestBase::RxJUCETestBase(const String& name, const std::vector<TestCase>& testCases)
: UnitTest(name), testCases(testCases)
{
}

void RxJUCETestBase::setTestToRun(int testIndex)
{
	this->testToRun = testIndex;
}

void RxJUCETestBase::runTest()
{
	beginTest(testCases[testToRun].first);
	testCases[testToRun].second();
}
