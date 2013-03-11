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

#include "data/communication/message/Changement.h"
#include "data/communication/message/AddBrancheChangement.h"

#include "data/Client.h"

Client::Client(const string& debugName) {
	_debugName = debugName;
	_dataTest = NULL;
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

void Client::addMarqueur(Donnee* donnee, int donneeTmpId) {
	MarqueurClient* marqueur = new MarqueurClient(donnee, donneeTmpId);
	_marqueurs[donnee] = marqueur;
}

void Client::collecteChangements(vector<Changement*>& changements) {
	map<Donnee*, MarqueurClient*>::iterator itMa;

	for(itMa = _marqueurs.begin() ; itMa != _marqueurs.end() ; itMa++) {
		MarqueurClient* marqueur = itMa->second;
		Donnee* donnee = marqueur->getDonnee();

		Branche* branche = dynamic_cast<Branche*>(donnee);
		Changement* changement = NULL;

		if(branche) {
			if(marqueur->getSentRevision() == MarqueurClient::MARQUEUR_REVISION_INIT) {
				vector<int> brancheId = branche->getBrancheFullId();
				changement = new AddBrancheChangement(brancheId, branche->getRevision());
			}

			if(changement) {
				changement->update(marqueur);
				changements.push_back(changement);
			}

			continue;
		}

		Valeur* valeur = dynamic_cast<Valeur*>(donnee);

		if(valeur) {
			continue;
		}
	}
}

void Client::sendData(ostringstream& out) {
	_dataTest = new string(out.str());
}

string Client::receiveData() {
	string result = *_dataTest;
	_dataTest = NULL;

	return result;
}
