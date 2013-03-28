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
#include "data/MarqueurDistant.h"

#include "data/communication/message/ServerToClient/UpdateValeurFromServerChangement.h"

UpdateValeurFromServerChangement::UpdateValeurFromServerChangement(istringstream& in) : Changement("UpdateValeurChangement") {
	unserialize(in);
}

UpdateValeurFromServerChangement::UpdateValeurFromServerChangement(const Valeur* valeur) : Changement("UpdateValeurChangement") {
	_brancheId = valeur->getBrancheId();
	_valeurId = valeur->getValeurId();

	_revision = valeur->getRevision();

	if(const ValeurInt* valeurInt = dynamic_cast<const ValeurInt*>(valeur)) {
		_valeur = new JktUtils::IntData(valeurInt->getValeur());
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Valeur de type inconnu";
	}
}

UpdateValeurFromServerChangement::~UpdateValeurFromServerChangement() {
	if(_valeur) {
		delete _valeur;
	}
};

void UpdateValeurFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void UpdateValeurFromServerChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)UPDATE_VALEUR_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, *_valeur);
}

void UpdateValeurFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

string UpdateValeurFromServerChangement::toString() {
	ostringstream str;

	str << "[" << _dataType;

	str << "  ; brancheId:";
	StreamUtils::writeHumanReadable(str, _brancheId);

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, *_valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& UpdateValeurFromServerChangement::getBrancheId() const {
	return _brancheId;
}

int UpdateValeurFromServerChangement::getValeurId() const {
	return _valeurId;
}

int UpdateValeurFromServerChangement::getRevision() const {
	return _revision;
}

const JktUtils::Data* UpdateValeurFromServerChangement::getValeur() const {
	return _valeur;
}
