/*
 * TableauTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TableauTest_H_
#define TableauTest_H_

#include <string>

#include "test/Test.h"

namespace jkt {

class TableauTest : public Test {
	void TableauIndexTest();

public:
	TableauTest();
	virtual ~TableauTest();

	std::string getName();
	std::string getDescription();

	void test();
};

} /* namespace jkt */
#endif /* TableauTest_H_ */
