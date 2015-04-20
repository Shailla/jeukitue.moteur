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
#include "data/ValeurInt.h"
#include "data/ServeurDataTree.h"
#include "data/MarqueurDistant.h"

#include "data/communication/message/ServerToClient/UpdateValeurFromServerChangement.h"

UpdateValeurFromServerChangement::UpdateValeurFromServerChangement(istringstream& in) : Changement("UpdateValChgt", PRIORITY_UpdateValeurFromServerChangement) {
	unserialize(in);
}

UpdateValeurFromServerChangement::UpdateValeurFromServerChangement(const vector<int>& parentBrancheId, int valeurId, int revision, JktUtils::AnyData valeur) : Changement("UpdateValChgt", PRIORITY_UpdateValeurFromServerChangement) {
	_parentBrancheId = parentBrancheId;
	_valeurId = valeurId;
	_revision = revision;
	_valeur = valeur;
}

UpdateValeurFromServerChangement::~UpdateValeurFromServerChangement() {
};

void UpdateValeurFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void UpdateValeurFromServerChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)UPDATE_VALEUR_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, _valeur);
}

void UpdateValeurFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

string UpdateValeurFromServerChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << "  ; parentBrancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, _valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& UpdateValeurFromServerChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int UpdateValeurFromServerChangement::getValeurId() const {
	return _valeurId;
}

int UpdateValeurFromServerChangement::getRevision() const {
	return _revision;
}

const JktUtils::AnyData UpdateValeurFromServerChangement::getValeur() const {
	return _valeur;
}

int UpdateValeurFromServerChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
