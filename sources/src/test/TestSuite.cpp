/*
 * TestSuite.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <exception>

#include "main/Cfg.h"
#include "exception/JktException.h"
#include "test/utils/TraceTest.h"
#include "test/utils/TableauTest.h"
#include "test/utils/UtilsTest.h"
#include "test/utils/RessourcesLoaderTest.h"
#include "test/reseau/JsonTest.h"
#include "test/reseau/TcpServerTest.h"
#include "test/reseau/UdpCommunicationTest.h"
#include "test/data/DataTreeTest.h"

#include "test/TestSuite.h"

using namespace std;

extern CCfg Config;		// Contient la configuration du jeu

namespace jkt {

TestSuite::TestSuite() {
	_tests.push_back(new UtilsTest());
	_tests.push_back(new TraceTest());
	_tests.push_back(new TableauTest());
	_tests.push_back(new RessourcesLoaderTest());
	_tests.push_back(new TcpServerTest());
	_tests.push_back(new JsonTest());
	_tests.push_back(new UdpCommunicationTest());
	_tests.push_back(new DataTreeTest());
}

TestSuite::~TestSuite() {
}

void TestSuite::init() {
	Config.Reseau.Init();	// Initialisation du r�seau
}

void TestSuite::launchTests() {
	vector<Test*>::iterator iter;
	int nbr = 1;
	int nbrOk = 0;
	int nbrKo = 0;
	string testName;

	cout << endl << "============================================================================";
	cout << endl << "  �X�CUTION DE " << _tests.size() << " TEST" << ((_tests.size()<=1)?"":"S");
	cout << endl << "============================================================================";
	cout << endl;

	for(iter = _tests.begin() ; iter != _tests.end() ; iter++) {
		try {
			Test* test = *iter;
			testName = test->getName();

			cout << endl << ">>>>>>>>>>>>>>>>>>>>>>>> " << nbr << " - LAUNCHING TEST '" << testName << "' >>>>>>>>>>>>>>>>>>>>>>>>";
			cout << endl << "Description of the test : " << test->getDescription() << endl;
			test->test();	// Ex�cution du test
			cout << endl << endl << "<<<<<<<<<<<<<<<<<<<<<<<< " << nbr << " - TEST '" << testName << "' OK <<<<<<<<<<<<<<<<<<<<<<<<";
			cout << endl << flush;

			_results.push_back(testName + " : OK");
			nbrOk++;
		}
		catch(TestError& error) {
			_results.push_back(testName + " : KO (error)");
			cout << endl << endl << ">>>>>> " << nbr << " - TEST '" << testName << "' KO (file = " << error._file << ", line = " << error._line << ", message = " << error._message << ") <<<<<<" << flush;
			nbrKo++;
		}
		catch(JktException& exception) {
			_results.push_back(testName + " : KO (JktException)");
			cout << endl << endl << ">>>>>> " << nbr << " - TEST '" << testName << "' KO (JktException : message = " << exception.what() << ") <<<<<<" << flush;
			nbrKo++;
		}
		catch(std::exception& exception) {
			_results.push_back(testName + " : KO (std::exception)");
			cout << endl << endl << ">>>>>> " << nbr << " - TEST '" << testName << "' KO (std::exception : message = " << exception.what() << ") <<<<<<" << flush;
			nbrKo++;
		}
		catch(std::string& exception) {
			_results.push_back(testName + " : KO (std::string)");
			cout << endl << endl << ">>>>>> " << nbr << " - TEST '" << testName << "' KO (std::string : message = " << exception << ") <<<<<<" << flush;
			nbrKo++;
		}
		catch(...) {
			_results.push_back(testName + " : KO (unexpected exception)");
			cout << endl << endl << ">>>>>> " << nbr << " - TEST '" << testName << "' KO (with unexpected exception) <<<<<<" << flush;
			nbrKo++;
		}

		nbr++;
	}

	cout << endl;
	cout << endl << "============================================================================";
	cout << endl << "  R�SULTATS";
	cout << endl << "============================================================================";
	cout << endl << "  D�tails :";
	for(vector<string>::iterator it = _results.begin() ; it != _results.end() ; it++) {
		cout << endl << "  \t- " << *it;
	}
	cout << endl << "----------------------------------------------------------------------------";
	cout << endl << "  Synth�se :";
	cout << endl << "  \t" << nbrOk << " test(s) r�ussi(s)";
	cout << endl << "  \t" << nbrKo << " test(s) en �chec";
	cout << endl << "  \tNombre total de test(s) : " << _tests.size();
	cout << endl;
	cout << endl;
	cout << endl;
}

} /* namespace jkt */
