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

AddValeurFromServerChangement::AddValeurFromServerChangement(istringstream& in) : Changement("AddValeurFromServerChangement", PRIORITY_AddValeurFromServerChangement) {
	unserialize(in);
}

AddValeurFromServerChangement::AddValeurFromServerChangement(const vector<int>& parentBrancheId, int valeurId, int revision, const string& valeurName, JktUtils::Data* valeur) : Changement("AddValeurFromServerChangement", PRIORITY_AddValeurFromServerChangement) {
	_parentBrancheId = parentBrancheId;
	_valeurId = valeurId;
	_revision = revision;
	_valeurName = valeurName;
	_valeur = valeur;
}

AddValeurFromServerChangement::~AddValeurFromServerChangement() {
	if(_valeur) {
		delete _valeur;
	}
};

void AddValeurFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddValeurFromServerChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, *_valeur);
}

void AddValeurFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
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
	StreamUtils::writeHumanReadable(str, *_valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AddValeurFromServerChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddValeurFromServerChangement::getValeurId() const {
	return _valeurId;
}

const string& AddValeurFromServerChangement::getValeurName() const {
	return _valeurName;
}

int AddValeurFromServerChangement::getRevision() const {
	return _revision;
}

const JktUtils::Data* AddValeurFromServerChangement::getValeur() const {
	return _valeur;
}

int AddValeurFromServerChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
