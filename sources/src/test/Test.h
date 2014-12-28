/*
 * Test.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TEST_H_
#define TEST_H_

#include <string>

#define ASSERT_TRUE(var, message) assertTrue(__FILE__, __LINE__, var, message)
#define ASSERT_FALSE(var, message) assertFalse(__FILE__, __LINE__, var, message)
#define ASSERT_NULL(var, message) assertNull(__FILE__, __LINE__, var, message)
#define ASSERT_NOT_NULL(var, message) assertNotNull(__FILE__, __LINE__, var, message)
#define ASSERT_EQUAL(expected, var, message) assertEqual(__FILE__, __LINE__, expected, var, message)

namespace JktTest {

class TestError {
public:
	std::string _file;
	int _line;
	std::string _message;

	TestError(const char* file, int line, const char* message) {
		_file = file;
		_line = line;

		if(message) {
			_message = message;
		}
	}

	TestError(const char* file, int line, const std::string message) {
		_file = file;
		_line = line;

		_message = message;
	}
};

class Test {
	string _testName;

public:
	Test(const string& testName);
	virtual ~Test();

	std::string getName();
	virtual std::string getDescription() = 0;
	virtual void test() = 0;

	// Assertion methods
	void assertTrue(const char* file, int line, bool var, const char* message = 0) throw(TestError);
	void assertFalse(const char* file, int line, bool var, const char* message = 0) throw(TestError);
	void assertNull(const char* file, int line, const void* var, const char* message = 0);
	void assertNotNull(const char* file, int line, const void* var, const char* message = 0);
	void assertEqual(const char* file, int line, const void* expected, const void* var, const char* message = 0);
	void assertEqual(const char* file, int line, const std::string& expected, const std::string& var, const char* message = 0);
	void assertEqual(const char* file, int line, const char* expected, const char* var, const char* message = 0);
	void assertEqual(const char* file, int line, int expected, int var, const char* message = 0);
	void assertEqual(const char* file, int line, float expected, float var, const char* message = 0);

	// Log methods
	void log(const char* message, int line);
	void log(const string& message, int line);
	void log(const ostringstream& message, int line);
};

} /* namespace JktTest */
#endif /* TEST_H_ */
