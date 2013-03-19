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

#include "data/communication/message/AddValeurChangement.h"

AddValeurChangement::AddValeurChangement(istringstream& in) {
	unserialize(in);
}

AddValeurChangement::AddValeurChangement(const vector<int>& brancheId, int valeurId, int revision, const string& valeurName, JktUtils::Data* valeur) {
	_brancheId = brancheId;
	_valeurId = valeurId;
	_revision = revision;
	_valeurName = valeurName;
	_valeur = valeur;
}

AddValeurChangement::~AddValeurChangement() {
	if(_valeur) {
		delete _valeur;
	}
};

void AddValeurChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddValeurChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, *_valeur);
}

void AddValeurChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	_valeur = StreamUtils::readData(in);
}

const std::vector<int>& AddValeurChangement::getBrancheId() const {
	return _brancheId;
}

int AddValeurChangement::getValeurId() const {
	return _valeurId;
}

const string& AddValeurChangement::getValeurName() const {
	return _valeurName;
}

int AddValeurChangement::getRevision() const {
	return _revision;
}

JktUtils::Data* AddValeurChangement::getValeur() const {
	return _valeur;
}
