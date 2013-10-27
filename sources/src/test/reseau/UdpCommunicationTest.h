/*
 * UdpCommunicationTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef UDPCOMMUNICATIONTEST_H_
#define UDPCOMMUNICATIONTEST_H_

#include "test/Test.h"

namespace JktTest {

class UdpCommunicationTest : public Test {
public:
	UdpCommunicationTest();
	virtual ~UdpCommunicationTest();

	string getName();
	void test();
};

} /* namespace JktUtils */
#endif /* UDPCOMMUNICATIONTEST_H_ */
