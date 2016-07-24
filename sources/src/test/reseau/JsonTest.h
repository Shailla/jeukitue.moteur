/*
 * JsonTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef JSONTEST_H_
#define JSONTEST_H_

#include <string>

#include "test/Test.h"

namespace jkt {

class JsonTest : public Test {
public:
	JsonTest();
	virtual ~JsonTest();

	std::string getDescription() override;

	void test() override;
};

} /* namespace jkt */
#endif /* JSONTEST_H_ */
