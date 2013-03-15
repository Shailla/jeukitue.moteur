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
#include "data/DataTree.h"

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
	int var = ADD_DATA_MESSAGE;
	out.write((char*)&var, sizeof(int));
	StreamUtils::write(out, _parentBrancheId);
	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _brancheName);
	out.write((char*)&_revision, sizeof(int));
}

void AddBrancheChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _parentBrancheId);
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _brancheName);
	in.read((char*)&_revision, sizeof(int));
}

void AddBrancheChangement::change(DataTree* tree) {
//	TODO tree->addBranche()
}
