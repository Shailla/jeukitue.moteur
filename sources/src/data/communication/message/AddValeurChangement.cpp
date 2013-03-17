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

AddValeurChangement::AddValeurChangement(const vector<int>& brancheId, int revision, const string& valeurName, int valeur) {
	_brancheId = brancheId;
	_revision = revision;
	_valeurName = valeurName;
	_valeur = valeur;
}

void AddValeurChangement::update(MarqueurClient* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddValeurChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_VALEUR_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurName);
	StreamUtils::write(out, _revision);
	StreamUtils::write(out, _valeur);
}

void AddValeurChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurName);
	StreamUtils::read(in, _revision);
	StreamUtils::read(in, _valeur);
}

const string& AddValeurChangement::getValeurName() const {
	return _valeurName;
}

const std::vector<int>& AddValeurChangement::getBrancheId() const {
	return _brancheId;
}

int AddValeurChangement::getRevision() const {
	return _revision;
}

int AddValeurChangement::getValeur() const {
	return _valeur;
}
