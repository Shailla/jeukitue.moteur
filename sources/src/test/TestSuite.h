/*
 * TestSuite.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include <vector>

#include "test/Test.h"

using namespace std;

namespace JktTest {

class TestSuite {
	vector<Test*> _tests;

public:
	TestSuite();
	virtual ~TestSuite();

	void init();
	void launchTests();
};

} /* namespace JktTest */
#endif /* TESTSUITE_H_ */
