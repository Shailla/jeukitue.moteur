/*
 * AcceptAddBrancheFromClientChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>
#include <iostream>

#include "util/StreamUtils.h"
#include "data/ValeurInt.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"

using namespace std;

AcceptAddValeurFromClientChangement::AcceptAddValeurFromClientChangement(istringstream& in) : Changement("AcceptAddValFromClChgt", PRIORITY_AcceptAddValeurFromClientChangement, 0) {
	unserialize(in);
}

AcceptAddValeurFromClientChangement::AcceptAddValeurFromClientChangement(const vector<int>& parentBrancheId, int valeurTmpId, int valeurId, int revision) : Changement("AcceptAddValFromClChgt", PRIORITY_AcceptAddValeurFromClientChangement, valeurTmpId) {
	_parentBrancheId = parentBrancheId;
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

void AcceptAddValeurFromClientChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ACCEPT_ADD_VALEUR_FROM_CLIENT);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _valeurTmpId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
}

void AcceptAddValeurFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _valeurTmpId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
	_minorPriority = _valeurTmpId;
}

string AcceptAddValeurFromClientChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << "; brancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; valeurTmpId:" << _valeurTmpId;

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AcceptAddValeurFromClientChangement::getParentBrancheId() const {
	return _parentBrancheId;
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

int AcceptAddValeurFromClientChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
