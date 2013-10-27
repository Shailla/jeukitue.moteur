/*
 * TestSuite.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>

using namespace std;

#include "main/Cfg.h"
#include "test/reseau/UdpCommunicationTest.h"

#include "test/TestSuite.h"

extern CCfg Config;		// Contient la configuration du jeu

namespace JktTest {

TestSuite::TestSuite() {
	_tests.push_back(new UdpCommunicationTest());
}

TestSuite::~TestSuite() {
}

void TestSuite::init() {
	Config.Reseau.Init();	// Initialisation du réseau
}

void TestSuite::launchTests() {
	vector<Test*>::iterator iter;
	int nbr = 1;
	int nbrOk = 0;
	string testName;

	cout << endl << "==============================================================";
	cout << endl << "  LANCEMENT DE L'EXECUTION DE " << _tests.size() << " TESTS";
	cout << endl << "==============================================================";
	cout << endl;

	for(iter = _tests.begin() ; iter != _tests.end() ; iter++) {
		try {
			Test* test = *iter;
			testName = test->getName();

			cout << endl << "=== " << nbr << " LAUNCHING TEST '" << testName << "'";
			test->test();
			cout << endl << "=== " << nbr << " TEST '" << testName << "' OK";

			nbrOk++;
		}
		catch(TestError& error) {
			cout << endl << "=== " << nbr << " TEST '" << testName << "' KO (file = " << error._file << ", line = " << error._line << ", message = " << error._message << ")";
		}
		catch(...) {
			cout << endl << "=== " << nbr << " TEST '" << testName << "' KO (with unexpected exception)";
		}

		nbr++;
	}

	cout << endl;
	cout << endl << "==============================================================";
	cout << endl << "  " << nbrOk << " TESTS REUSSIS SUR " << _tests.size() << " TESTS";
	cout << endl << "==============================================================";
}

} /* namespace JktTest */
