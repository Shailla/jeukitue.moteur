/*
 * Client.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>

using namespace std;

#include "data/Client.h"

Client::Client(const string& debugName) {
	_debugName = debugName;
}

Client::~Client() {
}

string Client::getDebugName() {
	return _debugName;
}

MarqueurClient* Client::getMarqueur(Donnee* donnee) {
	return _marqueurs.at(donnee);
}

map<Donnee*, MarqueurClient*>& Client::getMarqueurs() {
	return _marqueurs;
}

void Client::addMarqueur(Donnee* donnee, int donneeTmpId, bool isUpToDate) {
	MarqueurClient* marqueur = new MarqueurClient(donnee, donneeTmpId, isUpToDate);

	_marqueurs[donnee] = marqueur;
}
