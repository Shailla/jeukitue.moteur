/*
 * Test.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "SDL.h"

#include "test/Test.h"

namespace JktTest {

Test::Test(const string& testName) : _testName(testName) {
}

Test::~Test() {
}

string Test::getName() {
	return _testName;
}


void Test::assertTrue(const char* file, int line, bool var, const char* message) {
	if(!var) {
		throw TestError(file, line, message);
	}
}

void Test::assertFalse(const char* file, int line, bool var, const char* message) {
	if(var) {
		throw TestError(file, line, message);
	}
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
		stringstream msg;
		msg << message << " | " << "Valeur attendue [" << expected << "] obtenu [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, const string& expected, const string& var, const char* message) {
	if(expected != var) {
		stringstream msg;
		msg << message << " | " << "Valeur attendue [" << expected << "] obtenu [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, const char* expected, const char* var, const char* message) {
	if(!strcmp(expected, var)) {
		stringstream msg;
		msg << message << " | " << "Valeur attendue [" << expected << "] obtenu [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, int expected, int var, const char* message) {
	if(expected != var) {
		stringstream msg;
		msg << message << " | " << "Valeur attendue [" << expected << "] obtenu [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::log(const char* message) {
	cout << flush << endl << endl << "= " << SDL_GetTicks() << " TEST : " << message << flush;
}

void Test::log(const string& message) {
	cout << flush << endl << endl << "= " << SDL_GetTicks() << " TEST : " << message << flush;
}

void Test::log(const ostringstream& message) {
	cout << flush << endl << endl << "= " << SDL_GetTicks() << " TEST : " << message.str() << flush;
}

} /* namespace JktTest */
