/*
 * UtilsTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef UTILSTEST_H_
#define UTILSTEST_H_

#include <string>

#include "test/Test.h"

namespace jkt {

class UtilsTest : public Test {
	void stringUtilsTest();
	void containsTest();

public:
	UtilsTest();
	virtual ~UtilsTest();

	std::string getName();
	std::string getDescription();

	void test();
};

} /* namespace jkt */
#endif /* UTILSTEST_H_ */
