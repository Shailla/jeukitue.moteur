/*
 * Test.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>

#include "SDL.h"

#include "util/Trace.h"

#include "test/Test.h"

using namespace std;

namespace jkt {

Test::Test(const string& testName) : _testName(testName) {
}

Test::~Test() {
}

string Test::getName() {
	return _testName;
}


void Test::assertTrue(const char* file, int line, bool var, const char* message) throw(TestError) {
	if(!var) {
		throw TestError(file, line, message);
	}
}

void Test::assertFalse(const char* file, int line, bool var, const char* message) throw(TestError) {
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
		msg << message << " | " << "\nValeur attendue [" << expected << "]\nvaleur obtenue [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, const string& expected, const string& var, const char* message) {
	if(expected != var) {
		stringstream msg;
		msg << message << " | " << "\nValeur attendue : [" << expected << "]\nvaleur obtenue : [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, const char* expected, const char* var, const char* message) {
	if(!strcmp(expected, var)) {
		stringstream msg;
		msg << message << " | " << "\nValeur attendue : [" << expected << "]\nvaleur obtenue : [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, float expected, float var, const char* message) {
	if(expected != var) {
		stringstream msg;
		msg << message << " | " << "\nValeur attendue [" << expected << "]\nvaleur obtenue [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::assertEqual(const char* file, int line, int expected, int var, const char* message) {
	if(expected != var) {
		stringstream msg;
		msg << message << " | " << "\nValeur attendue [" << expected << "]\nvaleur obtenue [" << var << "]";
		throw TestError(file, line, msg.str());
	}
}

void Test::fail(const char* file, int line, const char* message) {
	throw TestError(file, line, message);
}

void Test::log(const char* message, int line) {
	ostringstream msg;

	msg << endl << "LOG [" << SDL_GetTicks() << " ms - " << line << "] " << message << flush;

	cout << msg.str() << flush;
	LOGINFO((msg.str().c_str()));
}

void Test::log(const string& message, int line) {
	ostringstream msg;

	msg << endl << "LOG [" << SDL_GetTicks() << " ms - " << line << "] " << message << flush;

	cout << msg.str() << flush;
	LOGINFO((msg.str().c_str()));
}

void Test::log(const ostringstream& message, int line) {
	ostringstream msg;

	msg << endl << "LOG [" << SDL_GetTicks() << " ms - " << line << "] " << message.str() << flush;

	cout << msg.str() << flush;
	LOGINFO((msg.str().c_str()));
}

} /* namespace jkt */
