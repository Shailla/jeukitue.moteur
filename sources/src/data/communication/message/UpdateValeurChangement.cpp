/*
 * AddBrancheChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

#include "util/StreamUtils.h"
#include "util/types/IntData.h"
#include "data/ValeurInt.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/UpdateValeurChangement.h"

UpdateValeurChangement::UpdateValeurChangement(istringstream& in) : Changement("UpdateValeurChangement") {
	unserialize(in);
}

UpdateValeurChangement::UpdateValeurChangement(Valeur* valeur) : Changement("UpdateValeurChangement") {
	_brancheId = valeur->getBrancheId();
	_valeurId = valeur->getValeurId();

	_revision = valeur->getRevision();

	if(ValeurInt* valeurInt = dynamic_cast<ValeurInt*>(valeur)) {
		_valeur = new JktUtils::IntData(valeurInt->getValeur());
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Valeur de type inconnu";
	}
}

UpdateValeurChangement::~UpdateValeurChangement() {
	if(_valeur) {
		delete _valeur;
	}
};

void UpdateValeurChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void UpdateValeurChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)UPDATE_VALEUR_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, *_valeur);
}

void UpdateValeurChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

const std::vector<int>& UpdateValeurChangement::getBrancheId() const {
	return _brancheId;
}

int UpdateValeurChangement::getValeurId() const {
	return _valeurId;
}

int UpdateValeurChangement::getRevision() const {
	return _revision;
}

JktUtils::Data* UpdateValeurChangement::getValeur() const {
	return _valeur;
}
