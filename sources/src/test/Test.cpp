/*
 * Test.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include "test/Test.h"

namespace JktTest {

Test::Test(const string& testName) : _testName(testName) {
}

Test::~Test() {
}

string Test::getName() {
	return _testName;
}

void Test::assertNull(const char* file, int line, const void* var, const char* message) {
	if(var) {
		throw TestError(file, line, message);
	}
}

void Test::assertNotNull(const char* file, int line, const void* var, const char* message) {
	if(!var) {
		throw TestError(file, line, message);
	}
}

void Test::assertEqual(const char* file, int line, const void* expected, const void* var, const char* message) {
	if(expected != var) {
		throw TestError(file, line, message);
	}
}

void Test::assertEqual(const char* file, int line, const string& expected, const string& var, const char* message) {
	if(expected != var) {
		throw TestError(file, line, message);
	}
}

void Test::assertEqual(const char* file, int line, const char* expected, const char* var, const char* message) {
	if(!strcmp(expected, var)) {
		throw TestError(file, line, message);
	}
}


} /* namespace JktTest */
