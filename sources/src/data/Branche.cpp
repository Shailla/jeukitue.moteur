/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Branche.h"

#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

Branche::Branche(int brancheId) {
	_brancheId = brancheId;
}

Branche::~Branche() {
}

Branche* Branche::getSubBranche(int brancheId) const {
	cout << endl << "JJJ : " << brancheId << flush;

	Branche* branche = NULL;

	try {
		branche = _subBranches.at(brancheId);
	}
	catch(out_of_range& exception) {
		cout << endl << "OUT_OF_RANGE_EXCEPTION" << flush;
		branche = NULL;
	}

	return branche;
}

Branche* Branche::createSubBranche() {
	// Alloue une référence pour la nouvelle branche
	int ref = _referenceGenerator.genRef();

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(ref);
	_subBranches[ref] = newBranche;

	return newBranche;
}

map<int, Branche*>& Branche::getSubBranches() {
	return _subBranches;
}

map<int, Valeur*>& Branche::getValeurs() {
	return _valeurs;
}

int Branche::getBrancheId() const {
	return _brancheId;
}
