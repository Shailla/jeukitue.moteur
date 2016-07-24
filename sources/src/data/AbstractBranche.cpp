/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

#include "util/CollectionsUtils.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/MarqueurDistant.h"

#include "data/AbstractBranche.h"

using namespace jkt;

AbstractBranche::AbstractBranche(DataTree* dataTree, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : Donnee(dataTree, revision, brancheType, UPDATE_MODE::ANY) {
	_parent = 0;
	_brancheId = brancheId;
	_brancheName = brancheName;
	_brancheTmpId = brancheTmpId;
}

AbstractBranche::AbstractBranche(AbstractBranche* parent, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : Donnee(parent->getDataTree(), revision, brancheType, UPDATE_MODE::ANY) {
	_parent = parent;
	_brancheId = brancheId;
	_brancheName = brancheName;
	_brancheTmpId = brancheTmpId;
}

AbstractBranche::~AbstractBranche() {
}

void AbstractBranche::setBrancheId(int brancheId) {
	_brancheId = brancheId;
}

string AbstractBranche::getBrancheName() const {
	return _brancheName;
}

int AbstractBranche::getBrancheId() const {
	return _brancheId;
}

int AbstractBranche::getBrancheTmpId() const {
	return _brancheTmpId;
}


vector<int> AbstractBranche::getParentBrancheId(void) const {
	vector<int> parentBrancheId;

	if(_parent) {
		parentBrancheId = _parent->getBrancheFullId();
	}
	else {
		// Do nothing, return empty id
	}

	return parentBrancheId;
}

vector<int> AbstractBranche::getParentBrancheIdOrTmpId(void) const {
	vector<int> parentBrancheIdOrTmpId;

	if(_parent) {
		parentBrancheIdOrTmpId = _parent->getBrancheFullIdOrTmpId();
	}
	else {
		// Do nothing, return empty id
	}

	return parentBrancheIdOrTmpId;
}

void AbstractBranche::getBrancheFullId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullId(id);
		id.push_back(_brancheId);
	}
}

void AbstractBranche::getBrancheFullIdOrTmpId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullIdOrTmpId(id);

		if(_brancheId >= 0) {
			id.push_back(_brancheId);
		}
		else {
			id.push_back(_brancheTmpId);	// On passe l'identifiant temporaire en nombre négatif pour qu'il puisse être reconnu comme tel
		}
	}
}

vector<int> AbstractBranche::getBrancheFullIdOrTmpId() const {
	vector<int> id;
	getBrancheFullIdOrTmpId(id);

	return id;
}

vector<int> AbstractBranche::getBrancheFullId() const {
	vector<int> id;
	getBrancheFullId(id);

	return id;
}

string AbstractBranche::print(DistantTreeProxy* distant, bool details, bool publicOnly, int indentation) {
	ostringstream out;
	print(out, distant, details, publicOnly, indentation);

	return out.str();
}
