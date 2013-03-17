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

#include "data/communication/message/AddBrancheChangement.h"

AddBrancheChangement::AddBrancheChangement(istringstream& in) {
	unserialize(in);
}

AddBrancheChangement::AddBrancheChangement(const vector<int>& parentBrancheId, int brancheId, int revision, const string& brancheName) {
	_parentBrancheId = parentBrancheId;
	_brancheId = brancheId;
	_revision = revision;
	_brancheName = brancheName;
}

void AddBrancheChangement::update(MarqueurClient* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)ADD_BRANCHE_MESSAGE);

	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _brancheName);
	StreamUtils::write(out, _revision);
}

void AddBrancheChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _brancheName);
	StreamUtils::read(in, _revision);
}

int AddBrancheChangement::getBrancheId() const {
	return _brancheId;
}

const string& AddBrancheChangement::getBrancheName() const {
	return _brancheName;
}

const std::vector<int>& AddBrancheChangement::getParentBrancheId() const {
	return _parentBrancheId;
}

int AddBrancheChangement::getRevision() const {
	return _revision;
}
