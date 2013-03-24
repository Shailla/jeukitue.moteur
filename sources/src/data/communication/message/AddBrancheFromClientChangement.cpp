/*
 * AddBrancheFromClientChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/AddBrancheFromClientChangement.h"

AddBrancheFromClientChangement::AddBrancheFromClientChangement(istringstream& in) : Changement("AddBrancheFromClientChangement") {
	unserialize(in);
}

AddBrancheFromClientChangement::AddBrancheFromClientChangement(const vector<int>& parentBrancheId, int brancheTmpId, int revision, const string& brancheName) : Changement("AddBrancheFromClientChangement") {
	_parentBrancheId = parentBrancheId;
	_brancheTmpId = brancheTmpId;
	_revision = revision;
	_brancheName = brancheName;
}

void AddBrancheFromClientChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheFromClientChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_BRANCHE_MESSAGE);

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
}

string AddBrancheFromClientChangement::toString() {
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
