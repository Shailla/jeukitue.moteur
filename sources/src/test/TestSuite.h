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

using namespace std;

namespace jkt {

class TestSuite {
	vector<Test*> _tests;
	vector<string> _results;

public:
	TestSuite();
	virtual ~TestSuite();

	void init();
	void launchTests();
};

} /* namespace jkt */
#endif /* TESTSUITE_H_ */
