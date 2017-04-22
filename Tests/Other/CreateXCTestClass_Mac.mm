//
//  CreateXCTestClass_Mac.m
//  RxJUCETests
//
//  Created by Martin Finke on 22.04.17.
//  Copyright © 2017 Martin Finke. All rights reserved.
//

#import "RxJUCETestBase.h"

#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#import <objc/runtime.h>

String SanitizeName(const String& name)
{
	// Replace spaces by underscore, only retain alphanumeric characters and underscore
	return name.trim().replaceCharacter(' ', '_').retainCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
}

String MethodNameForTestName(const String& testName)
{
	return "test_" + SanitizeName(testName);
}


@interface RxJUCETestCase : XCTestCase
@end

@interface RxJUCETestLoader : NSObject
@end

@implementation RxJUCETestCase

+ (NSBundle *)bundleForClass
{
	return [NSBundle bundleForClass:RxJUCETestLoader.class];
}

@end

@implementation RxJUCETestLoader

+ (void)load
{
	NSBundle *bundle = [NSBundle bundleForClass:self];
	[[NSNotificationCenter defaultCenter] addObserverForName:NSBundleDidLoadNotification object:bundle queue:nil usingBlock:^(NSNotification *notification) {
		[self registerTestClasses];
	}];
}

+ (void)registerTestClasses
{
	static UnitTestRunner testRunner;
	for (UnitTest *test : UnitTest::getAllTests()) {
		auto rxJuceTest = dynamic_cast<RxJUCETestBase*>(test);
		assert(rxJuceTest);
		Class testClass = objc_allocateClassPair(RxJUCETestCase.class, SanitizeName(rxJuceTest->getName()).toUTF8(), 0);
		NSAssert(testClass, @"Failed to create Objective C test class.");
		for (int testIndex = 0; testIndex < rxJuceTest->testCases.size(); ++testIndex) {
			const auto& testCase = rxJuceTest->testCases[testIndex];
			const String methodName = MethodNameForTestName(testCase.first);
			SEL selector = sel_registerName(methodName.toUTF8());
			IMP implementation = imp_implementationWithBlock(^(id s, SEL _c) {
				rxJuceTest->setTestToRun(testIndex);
				testRunner.runTests({test});
			});
			
			BOOL added = class_addMethod(testClass, selector, implementation, "v@:");
			NSAssert(added, @"Failed to add test method: %@.", methodName.toCFString());
		}
	}
}


@end


void* CreateXCTestClass(const char* name)
{
	return 0;
}
