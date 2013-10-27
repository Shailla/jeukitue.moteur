/*
 * Test.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TEST_H_
#define TEST_H_

#include <string>

using namespace std;

#define ASSERT_NULL(var) assertNull(__FILE__, __LINE__, var)
#define ASSERT_NOT_NULL(var) assertNotNull(__FILE__, __LINE__, var)
#define ASSERT_EQUAL(expected, var) assertEqual(__FILE__, __LINE__, expected, var)

namespace JktTest {

class TestError {
public:
	string _file;
	int _line;
	string _message;

	TestError(const char* file, int line, const char* message) {
		_file = file;
		_line = line;

		if(message) {
			_message = message;
		}
	}
};

class Test {
	string _testName;

public:
	Test(const string& testName);
	virtual ~Test();

	string getName();
	virtual void test() = 0;

	void assertNull(const char* file, int line, const void* var, const char* message = 0);
	void assertNotNull(const char* file, int line, const void* var, const char* message = 0);
	void assertEqual(const char* file, int line, const void* expected, const void* var, const char* message = 0);
	void assertEqual(const char* file, int line, const string& expected, const string& var, const char* message = 0);
	void assertEqual(const char* file, int line, const char* expected, const char* var, const char* message = 0);
};

} /* namespace JktTest */
#endif /* TEST_H_ */
