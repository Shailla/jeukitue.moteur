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

#include "util/types/Data.h"
#include "util/types/IntData.h"
#include "util/CollectionsUtils.h"
#include "data/communication/message/Changement.h"
#include "data/communication/message/AddBrancheChangement.h"
#include "data/communication/message/AddValeurChangement.h"
#include "data/communication/message/UpdateValeurChangement.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/Branche.h"

#include "data/Distant.h"

Distant::Distant(const string& debugName) {
	_debugName = debugName;
	_dataFromServerTest = NULL;
	_dataToServerTest = NULL;
}

Distant::~Distant() {
}

string& Distant::getDebugName() {
	return _debugName;
}

MarqueurDistant* Distant::getMarqueur(Donnee* donnee) {
	MarqueurDistant* marqueur;

	try {
		marqueur = _marqueurs.at(donnee);
	}
	catch(out_of_range& exception) {
		marqueur = NULL;
	}

	return marqueur;
}

map<Donnee*, MarqueurDistant*>& Distant::getMarqueurs() {
	return _marqueurs;
}

MarqueurDistant* Distant::addMarqueur(Donnee* donnee, int donneeTmpId) {
	MarqueurDistant* marqueur = new MarqueurDistant(donnee, donneeTmpId);
	_marqueurs[donnee] = marqueur;

	return marqueur;
}

void Distant::collecteChangements(vector<Changement*>& changements) {
	map<Donnee*, MarqueurDistant*>::iterator itMa;

	for(itMa = _marqueurs.begin() ; itMa != _marqueurs.end() ; itMa++) {
		MarqueurDistant* marqueur = itMa->second;
		Donnee* donnee = marqueur->getDonnee();

		Branche* branche = dynamic_cast<Branche*>(donnee);
		Changement* changement = NULL;

		if(branche) {
			// NOUVELLE BRANCHE : branche présente sur le serveur mais dont le client n'a pas connaissance
			if(marqueur->getConfirmedRevision() == MarqueurDistant::MARQUEUR_REVISION_INIT) {
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

			// NOUVELLE VALEUR : branche présente sur le serveur mais dont le client n'a pas connaissance
			if(marqueur->getConfirmedRevision() == MarqueurDistant::MARQUEUR_REVISION_INIT) {
				if(ValeurInt* valeurInt = dynamic_cast<ValeurInt*>(valeur)) {
					JktUtils::Data* data = new JktUtils::IntData(valeurInt->getValeur());
					changement = new AddValeurChangement(valeurInt->getBrancheId(), valeurInt->getValeurId(), valeurInt->getRevision(), valeurInt->getValeurName(), data);
				}
			}
			// VALEUR MODIFIEE : branche présente sur le serveur mais dont le client n'a pas connaissance
			else if(marqueur->getConfirmedRevision() != valeur->getRevision()) {
				changement = new UpdateValeurChangement(valeur);
			}

			if(changement) {
				changement->update(marqueur);
				changements.push_back(changement);
			}
		}
	}
}

void Distant::setDataFromServer(ostringstream& out) {
	if(out.str().size()) {
		_dataFromServerTest = new string(out.str());
	}
}

string* Distant::getDataFromServer(void) {
	string* var = _dataFromServerTest;
	_dataFromServerTest = NULL;

	return var;
}

void Distant::setDataToServer(string* data) {
	if(data) {
		_dataToServerTest = data;
	}
}

string* Distant::getDataToServer(void) {
	string* var = _dataToServerTest;
	_dataToServerTest = NULL;

	return var;
}
