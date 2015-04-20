/*
 * UpdateValeurFromClientChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/ClientToServer/UpdateValeurFromClientChangement.h"

UpdateValeurFromClientChangement::UpdateValeurFromClientChangement(istringstream& in) : Changement("UpdateValFromClChgt", PRIORITY_UpdateValeurFromClientChangement) {
	unserialize(in);
}

UpdateValeurFromClientChangement::UpdateValeurFromClientChangement(const vector<int>& parentBrancheId, int valeurId, int revision, JktUtils::AnyData valeur) : Changement("UpdateValFromClChgt", PRIORITY_UpdateValeurFromClientChangement) {
	_parentBrancheId = parentBrancheId;
	_valeurId = valeurId;
	_revision = revision;
	_valeur = valeur;
}

UpdateValeurFromClientChangement::~UpdateValeurFromClientChangement() {
};

void UpdateValeurFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void UpdateValeurFromClientChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)UPDATE_VALEUR_FROM_CLIENT_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, _valeur);
}

void UpdateValeurFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

string UpdateValeurFromClientChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << " brancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, _valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& UpdateValeurFromClientChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int UpdateValeurFromClientChangement::getValeurId() const {
	return _valeurId;
}

int UpdateValeurFromClientChangement::getRevision() const {
	return _revision;
}

JktUtils::AnyData UpdateValeurFromClientChangement::getValeur() const {
	return _valeur;
}

int UpdateValeurFromClientChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
