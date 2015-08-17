/*
 * AddValeurFromClientChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"

AddValeurFromClientChangement::AddValeurFromClientChangement(istringstream& in) : Changement("AddValFromClChgt", PRIORITY_AddValeurFromClientChangement, 0) {
	unserialize(in);
}

AddValeurFromClientChangement::AddValeurFromClientChangement(const vector<int>& parentBrancheId, int valeurTmpId, int revision, const string& valeurName, JktUtils::AnyData valeur) : Changement("AddValFromClChgt", PRIORITY_AddValeurFromClientChangement, valeurTmpId) {
	_parentBrancheId = parentBrancheId;
	_valeurTmpId = valeurTmpId;
	_revision = revision;
	_valeurName = valeurName;
	_valeur = valeur;
}

AddValeurFromClientChangement::~AddValeurFromClientChangement() {
};

void AddValeurFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddValeurFromClientChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_FROM_CLIENT_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurTmpId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, _valeur);
}

void AddValeurFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurTmpId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
	_minorPriority = _valeurTmpId;
}

string AddValeurFromClientChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << " brancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; valeurTmpId:" << _valeurTmpId;

	str << "; valeurName:" << _valeurName;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, _valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AddValeurFromClientChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddValeurFromClientChangement::getValeurTmpId() const {
	return _valeurTmpId;
}

const string& AddValeurFromClientChangement::getValeurName() const {
	return _valeurName;
}

int AddValeurFromClientChangement::getRevision() const {
	return _revision;
}

JktUtils::AnyData AddValeurFromClientChangement::getValeur() const {
	return _valeur;
}

int AddValeurFromClientChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
