/*
 * UtilsTest.h
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#ifndef RESSOURCESLOADERTEST_H_
#define RESSOURCESLOADERTEST_H_

#include <string>

#include "test/Test.h"

namespace jkt {

class RessourcesLoaderTest : public Test {
public:
	RessourcesLoaderTest();
	virtual ~RessourcesLoaderTest();

	std::string getName();
	std::string getDescription();

	void test();
};

} /* namespace jkt */
#endif /* RESSOURCESLOADERTEST_H_ */
