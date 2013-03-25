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

AddValeurFromServerChangement::AddValeurFromServerChangement(istringstream& in) : Changement("AddValeurFromServerChangement") {
	unserialize(in);
}

AddValeurFromServerChangement::AddValeurFromServerChangement(const vector<int>& brancheId, int valeurId, int revision, const string& valeurName, JktUtils::Data* valeur) : Changement("AddValeurFromServerChangement") {
	_brancheId = brancheId;
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

void AddValeurFromServerChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, *_valeur);
}

void AddValeurFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

string AddValeurFromServerChangement::toString() {
	ostringstream str;

	str << "[" << _dataType;

	str << " brancheId:";
	StreamUtils::writeHumanReadable(str, _brancheId);

	str << "; valeurId:" << _valeurId;

	str << "; valeurName:" << _valeurName;

	str << "; revision:" << _revision;

	str << "; data:";
	StreamUtils::writeHumanReadable(str, *_valeur);

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AddValeurFromServerChangement::getBrancheId() const {
	return _brancheId;
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

JktUtils::Data* AddValeurFromServerChangement::getValeur() const {
	return _valeur;
}
