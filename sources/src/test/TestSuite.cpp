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
#include "test/data/DataTreeTest.h"

#include "test/TestSuite.h"

extern CCfg Config;		// Contient la configuration du jeu

namespace JktTest {

TestSuite::TestSuite() {
	_tests.push_back(new UdpCommunicationTest());
	_tests.push_back(new DataTreeTest());
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
	int nbrKo = 0;
	string testName;

	cout << endl << "============================================================================";
	cout << endl << "  LANCEMENT DE L'EXECUTION DE " << _tests.size() << " TESTS";
	cout << endl << "============================================================================";
	cout << endl;

	for(iter = _tests.begin() ; iter != _tests.end() ; iter++) {
		try {
			Test* test = *iter;
			testName = test->getName();

			cout << endl << "<<<<<< " << nbr << " LAUNCHING TEST '" << testName << "' >>>>>>";
			test->test();
			cout << endl << endl << ">>>>>> " << nbr << " TEST '" << testName << "' OK <<<<<<";

			nbrOk++;
		}
		catch(TestError& error) {
			cout << endl << endl << ">>>>>> " << nbr << " TEST '" << testName << "' KO (file = " << error._file << ", line = " << error._line << ", message = " << error._message << ") <<<<<<";
			nbrKo++;
		}
//		catch(...) {
//			cout << endl << endl << ">>>>>> " << nbr << " TEST '" << testName << "' KO (with unexpected exception) <<<<<<";
//			nbrKo++;
//		}

		nbr++;
	}

	cout << endl;
	cout << endl << "============================================================================";
	cout << endl << "  RESULTATS :";
	cout << endl << "  \t" << nbrOk << " test(s) réussi(s)";
	cout << endl << "  \t" << nbrKo << " test(s) en échec";
	cout << endl << "  \tNombre total de test(s) : " << _tests.size();
	cout << endl << "============================================================================";
}

} /* namespace JktTest */
