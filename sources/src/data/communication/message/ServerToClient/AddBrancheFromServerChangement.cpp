/*
 * AddBrancheChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

#include "util/StreamUtils.h"
#include "data/ServeurDataTree.h"
#include "data/MarqueurDistant.h"

#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"

using namespace std;

AddBrancheFromServerChangement::AddBrancheFromServerChangement(istringstream& in) : Changement("AddBrFromSrvChgt", PRIORITY_AddBrancheFromServerChangement, 0) {
	unserialize(in);
}

AddBrancheFromServerChangement::AddBrancheFromServerChangement(const vector<int>& parentBrancheId, int brancheId, DONNEE_TYPE brancheType, int revision, const string& brancheName) : Changement("AddBrFromSrvChgt", PRIORITY_AddBrancheFromServerChangement, brancheId) {
	_parentBrancheId = parentBrancheId;
	_brancheId = brancheId;
	_brancheType = brancheType;
	_revision = revision;
	_brancheName = brancheName;
}

void AddBrancheFromServerChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
	marqueur->setConfirmedRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheFromServerChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)ADD_BRANCHE_FROM_SERVER_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _brancheType);
	StreamUtils::write(out, _brancheName);
	StreamUtils::write(out, _revision);
}

void AddBrancheFromServerChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, (int&)_brancheType);
	StreamUtils::read(in, _brancheName);
	StreamUtils::read(in, _revision);
	_minorPriority = _brancheId;
}

string AddBrancheFromServerChangement::toString() const {
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

DONNEE_TYPE AddBrancheFromServerChangement::getBrancheType() const {
	return _brancheType;
}

int AddBrancheFromServerChangement::getRevision() const {
	return _revision;
}

int AddBrancheFromServerChangement::getRootDistance() const {
	return _parentBrancheId.size();
}
