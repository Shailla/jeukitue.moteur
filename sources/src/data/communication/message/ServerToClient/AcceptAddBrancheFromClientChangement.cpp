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
#include "data/MarqueurDistant.h"

#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"

using namespace std;

AcceptAddBrancheFromClientChangement::AcceptAddBrancheFromClientChangement(istringstream& in) : Changement("AcceptAddBrFromClChgt", PRIORITY_AcceptAddBrancheFromClientChangement, 0) {
	unserialize(in);
}

AcceptAddBrancheFromClientChangement::AcceptAddBrancheFromClientChangement(const vector<int>& parentBrancheId, int brancheTmpId, int brancheId, int revision) : Changement("AcceptAddBrFromClChgt", PRIORITY_AcceptAddBrancheFromClientChangement, brancheTmpId) {
	_parentBrancheId = parentBrancheId;
	_brancheTmpId = brancheTmpId;
	_brancheId = brancheId;
	_revision = revision;
}

AcceptAddBrancheFromClientChangement::~AcceptAddBrancheFromClientChangement() {
};

void AcceptAddBrancheFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AcceptAddBrancheFromClientChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ACCEPT_ADD_BRANCHE_FROM_CLIENT);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheTmpId);
	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _revision);
}

void AcceptAddBrancheFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheTmpId);
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _revision);
	_minorPriority = _brancheTmpId;
}

string AcceptAddBrancheFromClientChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << "; _parentBrancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; brancheTmpId:" << _brancheTmpId;

	str << "; brancheId:" << _brancheId;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& AcceptAddBrancheFromClientChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AcceptAddBrancheFromClientChangement::getBrancheTmpId() const {
	return _brancheTmpId;
}

int AcceptAddBrancheFromClientChangement::getBrancheId() const {
	return _brancheId;
}

int AcceptAddBrancheFromClientChangement::getRevision() const {
	return _revision;
}

int AcceptAddBrancheFromClientChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
