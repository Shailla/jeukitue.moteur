/*
 * AddBrancheFromClientChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"

using namespace std;

AddBrancheFromClientChangement::AddBrancheFromClientChangement(istringstream& in) : Changement("AddBrFromClChgt", PRIORITY_AddBrancheFromClientChangement, 0) {
	unserialize(in);
}

AddBrancheFromClientChangement::AddBrancheFromClientChangement(const vector<int>& parentBrancheId, int brancheTmpId, int revision, const string& brancheName) : Changement("AddBrFromClChgt", PRIORITY_AddBrancheFromClientChangement, brancheTmpId) {
	_parentBrancheId = parentBrancheId;
	_brancheTmpId = brancheTmpId;
	_revision = revision;
	_brancheName = brancheName;
}

void AddBrancheFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheFromClientChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ADD_BRANCHE_FROM_CLIENT_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheTmpId);
	StreamUtils::write(out, _brancheName);
	StreamUtils::write(out, _revision);
}

void AddBrancheFromClientChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheTmpId);
	StreamUtils::read(in, _brancheName);
	StreamUtils::read(in, _revision);

	_minorPriority = _brancheTmpId;
}

string AddBrancheFromClientChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << " parentBrancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; brancheTmpId:" << _brancheTmpId;

	str << "; brancheName:" << _brancheName;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

int AddBrancheFromClientChangement::getBrancheTmpId() const {
	return _brancheTmpId;
}

const string& AddBrancheFromClientChangement::getBrancheName() const {
	return _brancheName;
}

const std::vector<int>& AddBrancheFromClientChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddBrancheFromClientChangement::getRevision() const {
	return _revision;
}

int AddBrancheFromClientChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
