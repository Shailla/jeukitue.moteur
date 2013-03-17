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

#include "util/CollectionsUtils.h"
#include "data/communication/message/Changement.h"
#include "data/communication/message/AddBrancheChangement.h"
#include "data/communication/message/AddValeurChangement.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/Branche.h"

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
	}
	catch(out_of_range& exception) {
		marqueur = NULL;
	}

	return marqueur;
}

map<Donnee*, MarqueurClient*>& Client::getMarqueurs() {
	return _marqueurs;
}

MarqueurClient* Client::addMarqueur(Donnee* donnee, int donneeTmpId) {
	MarqueurClient* marqueur = new MarqueurClient(donnee, donneeTmpId);
	_marqueurs[donnee] = marqueur;

	return marqueur;
}

void Client::collecteChangements(vector<Changement*>& changements) {
	map<Donnee*, MarqueurClient*>::iterator itMa;

	for(itMa = _marqueurs.begin() ; itMa != _marqueurs.end() ; itMa++) {
		MarqueurClient* marqueur = itMa->second;
		Donnee* donnee = marqueur->getDonnee();

		Branche* branche = dynamic_cast<Branche*>(donnee);
		Changement* changement = NULL;

		if(branche) {
			// NOUVELLE BRANCHE : branche présente sur le serveur mais dont le client n'a pas connaissance
			if(marqueur->getSentRevision() == MarqueurClient::MARQUEUR_REVISION_INIT) {
				changement = new AddBrancheChangement(branche->getParentBrancheId(), branche->getBrancheId(), branche->getRevision(), branche->getBrancheName());
			}

			if(changement) {
				changement->update(marqueur);
				changements.push_back(changement);
			}

			continue;
		}
		else {
			Valeur* valeur = (Valeur*)(donnee);

			if(marqueur->getSentRevision() == MarqueurClient::MARQUEUR_REVISION_INIT) {

				if(ValeurInt* valeurInt = dynamic_cast<ValeurInt*>(valeur)) {
					changement = new AddValeurChangement(valeurInt->getBrancheId(), valeurInt->getValeurId(), valeurInt->getRevision(), valeurInt->getValeurName(), valeurInt->getValeur());
				}
			}

			if(changement) {
				changement->update(marqueur);
				changements.push_back(changement);
			}
		}
	}
}

void Client::sendData(ostringstream& out) {
	if(out.str().size()) {
		_dataTest = new string(out.str());
	}
}

string* Client::getDataToSend(void) {
	string* var = _dataTest;

	_dataTest = NULL;

	return var;
}
