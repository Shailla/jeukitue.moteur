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

#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"

AddBrancheFromServerChangement::AddBrancheFromServerChangement(istringstream& in) : Changement("AddBrancheFromServerChangement") {
	unserialize(in);
}

AddBrancheFromServerChangement::AddBrancheFromServerChangement(const vector<int>& parentBrancheId, int brancheId, int revision, const string& brancheName) : Changement("AddBrancheFromServerChangement") {
	_parentBrancheId = parentBrancheId;
	_brancheId = brancheId;
	_revision = revision;
	_brancheName = brancheName;
}

void AddBrancheFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheFromServerChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_BRANCHE_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _brancheName);
	StreamUtils::write(out, _revision);
}

void AddBrancheFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _brancheName);
	StreamUtils::read(in, _revision);
}

string AddBrancheFromServerChangement::toString() {
	ostringstream str;

	str << "[" << _dataType;

	str << " parentBrancheId:";
	StreamUtils::writeHumanReadable(str, _parentBrancheId);

	str << "; brancheId:" << _brancheId;

	str << "; brancheName:" << _brancheName;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

int AddBrancheFromServerChangement::getBrancheId() const {
	return _brancheId;
}

const string& AddBrancheFromServerChangement::getBrancheName() const {
	return _brancheName;
}

const std::vector<int>& AddBrancheFromServerChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddBrancheFromServerChangement::getRevision() const {
	return _revision;
}
