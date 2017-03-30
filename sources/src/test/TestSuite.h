/*
 * TestSuite.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include <vector>
#include <string>

#include "test/Test.h"

namespace jkt {

class TestSuite {
	std::vector<Test*> _tests;
	std::vector<std::string> _results;

public:
	TestSuite();
	virtual ~TestSuite();

	void init();
	void launchTests();
};

} /* namespace jkt */
#endif /* TESTSUITE_H_ */
