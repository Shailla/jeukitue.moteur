/*
 * TraceTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <sstream>
#include <string>

#include "SDL.h"

#include "util/Trace.h"
#include "test/utils/TraceTest.h"

using namespace std;
using namespace jkt;

namespace jkt {

TraceTest::TraceTest() : Test("TraceTest") {
}

TraceTest::~TraceTest() {
}

string TraceTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Test de performance d'écriture de log d'erreur";

	return description;
}

void TraceTest::test() {
	Uint32 before = SDL_GetTicks();
	int nombre = 10000;

	for(int i=0 ; i<nombre ; i++) {
		LOGERROR(("Trace N°%d", i));
	}

	Uint32 after = SDL_GetTicks();

	stringstream msg;
	msg << "Temps d'écriture de " << nombre << " traces : " << (after - before) << " ms";

	log(msg.str(), __LINE__);
}

} // JktTest
