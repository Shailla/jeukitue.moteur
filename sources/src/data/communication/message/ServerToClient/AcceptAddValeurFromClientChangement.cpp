/*
 * AcceptAddBrancheFromClientChangement.cpp
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

#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"

AcceptAddValeurFromClientChangement::AcceptAddValeurFromClientChangement(istringstream& in) : Changement("AcceptAddValeurFromClientChangement") {
	unserialize(in);
}

AcceptAddValeurFromClientChangement::AcceptAddValeurFromClientChangement(const vector<int>& brancheId, int valeurTmpId, int valeurId, int revision) : Changement("AcceptAddValeurFromClientChangement") {
	_brancheId = brancheId;
	_valeurTmpId = valeurTmpId;
	_valeurId = valeurId;
	_revision = revision;
}

AcceptAddValeurFromClientChangement::~AcceptAddValeurFromClientChangement() {
};

void AcceptAddValeurFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AcceptAddValeurFromClientChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ACCEPT_ADD_VALEUR_FROM_CLIENT);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurTmpId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
}

void AcceptAddValeurFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurTmpId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
}

string AcceptAddValeurFromClientChangement::toString() {
	ostringstream str;

	str << "[" << _dataType;

	str << "; brancheId:";
	StreamUtils::writeHumanReadable(str, _brancheId);

	str << "; valeurTmpId:" << _valeurTmpId;

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AcceptAddValeurFromClientChangement::getBrancheId() const {
	return _brancheId;
}

int AcceptAddValeurFromClientChangement::getValeurTmpId() const {
	return _valeurTmpId;
}

int AcceptAddValeurFromClientChangement::getValeurId() const {
	return _valeurId;
}

int AcceptAddValeurFromClientChangement::getRevision() const {
	return _revision;
}
