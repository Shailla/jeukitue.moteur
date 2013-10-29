/*
 * DataTreeTest.h
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#ifndef DATATREETEST_H_
#define DATATREETEST_H_

#include <string>

#include "test/Test.h"

namespace JktTest {

class DataTreeTest : public Test {
public:
	DataTreeTest();
	virtual ~DataTreeTest();

	std::string getName();
	void test();
};

} /* namespace JktTest */

#endif /* DATATREETEST_H_ */
