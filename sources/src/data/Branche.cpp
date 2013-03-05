/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Branche.h"

#include <string>

using namespace std;

Branche::Branche(int brancheId) {
	_brancheId = brancheId;
}

Branche::~Branche() {
}

Branche* Branche::getSubBranche(int brancheId) const {
	return _subBranches.at(brancheId);
}

Branche* Branche::createSubBranche() {
	// Alloue une référence pour la nouvelle branche
	int ref = _referenceGenerator.genRef();

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(ref);
	_subBranches[ref] = newBranche;

	return newBranche;
}

