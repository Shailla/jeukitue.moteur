/*
 * Client.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "data/Client.h"

Client::Client(const string& debugName) {
	_debugName = debugName;
}

Client::~Client() {
}

string& Client::getDebugName() {
	return _debugName;
}

MarqueurClient* Client::getMarqueur(Donnee* donnee) {
	MarqueurClient* marqueur;

	try {
		marqueur = _marqueurs.at(donnee);
		cout << endl << "TAILLE : " << _marqueurs.size();
	}
	catch(out_of_range& exception) {
		marqueur = NULL;
	}

	return marqueur;
}

map<Donnee*, MarqueurClient*>& Client::getMarqueurs() {
	return _marqueurs;
}

void Client::addMarqueur(Donnee* donnee, int donneeTmpId, bool isUpToDate) {
	MarqueurClient* marqueur = new MarqueurClient(donnee, donneeTmpId, isUpToDate);
	_marqueurs[donnee] = marqueur;
}

void Client::collecteChangements(vector<MarqueurClient*>& changements) {
	map<Donnee*, MarqueurClient*>::iterator marqIter;

	for(marqIter = _marqueurs.begin() ; marqIter != _marqueurs.end() ; marqIter++) {
		MarqueurClient* marqueur = marqIter->second;

		if(!marqueur->isUpToDate()) {
			changements.push_back(marqueur);
		}
	}

}
