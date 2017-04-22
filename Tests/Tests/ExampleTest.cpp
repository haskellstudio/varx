//
//  ExampleTest.cpp
//  RxJUCETests
//
//  Created by Martin Finke on 22.04.17.
//  Copyright © 2017 Martin Finke. All rights reserved.
//

#include "RxJUCETestBase.h"

RxJUCETest("My Example Test",
	{"This is the first test", [this]() {
		expectEquals(1, 1);
	}},
			   
	{"This is another test", [this]() {
		expectEquals(1, 1);
	}}
);

RxJUCETest("Another Example Test",
	{"This is the first test", [this]() {
		expectEquals(2, 1);
	}},
		   
	{"This is another test", [this]() {
		expectEquals(1, 1);
	}}
);
