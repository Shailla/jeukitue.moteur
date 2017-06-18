/*
 * TraceTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef TRACETEST_H_
#define TRACETEST_H_

#include <string>

#include "test/Test.h"

namespace jkt {

class TraceTest : public Test {
public:
	TraceTest();
	virtual ~TraceTest();

	std::string getDescription() override;

	void test() override;
};

} /* namespace jkt */
#endif /* TRACETEST_H_ */
