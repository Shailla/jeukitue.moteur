/*
 * AddBrancheChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"
#include "data/MarqueurDistant.h"

#include "data/communication/message/ServerToClient/AddValeurFromServerChangement.h"

AddValeurFromServerChangement::AddValeurFromServerChangement(istringstream& in) : Changement("AddValFromSrvChgt", PRIORITY_AddValeurFromServerChangement, 0) {
	unserialize(in);
}

AddValeurFromServerChangement::AddValeurFromServerChangement(const vector<int>& parentBrancheId, int valeurId, UPDATE_MODE updateMode, int revision, const string& valeurName, jkt::AnyData valeur) : Changement("AddValFromSrvChgt", PRIORITY_AddValeurFromServerChangement, valeurId) {
	_parentBrancheId = parentBrancheId;
	_valeurId = valeurId;
	_updateMode = updateMode;
	_revision = revision;
	_valeurName = valeurName;
	_valeur = valeur;
}

AddValeurFromServerChangement::~AddValeurFromServerChangement() {
};

void AddValeurFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
	marqueur->setConfirmedRevision(marqueur->getDonnee()->getRevision());
}

void AddValeurFromServerChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, _valeur);
}

void AddValeurFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
	_minorPriority = _valeurId;
}

string AddValeurFromServerChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << " brancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; valeurId:" << _valeurId;

	str << "; valeurName:" << _valeurName;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, _valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AddValeurFromServerChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddValeurFromServerChangement::getValeurId() const {
	return _valeurId;
}

UPDATE_MODE AddValeurFromServerChangement::getUpdateMode() const {
	return _updateMode;
}

const string& AddValeurFromServerChangement::getValeurName() const {
	return _valeurName;
}

int AddValeurFromServerChangement::getRevision() const {
	return _revision;
}

const jkt::AnyData AddValeurFromServerChangement::getValeur() const {
	return _valeur;
}

int AddValeurFromServerChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
