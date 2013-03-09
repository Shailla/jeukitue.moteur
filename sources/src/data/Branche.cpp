/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "data/ValeurInt.h"

#include "data/Branche.h"

Branche::Branche(int brancheId, const string& brancheName) {
	_brancheId = brancheId;
	_brancheName = brancheName;
}

Branche::~Branche() {
}

Branche* Branche::getSubBranche(int brancheId) const {
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

Branche* Branche::createSubBranche(const string& brancheName) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(ref, brancheName);
	_subBranches[ref] = newBranche;

	return newBranche;
}

Valeur* Branche::createValeurInt(const string& valeurName, int valeur) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Valeur* newValeur = new ValeurInt(ref, valeurName, valeur);
	_valeurs[ref] = newValeur;

	return newValeur;
}

map<int, Branche*>& Branche::getSubBranches() {
	return _subBranches;
}

map<int, Valeur*>& Branche::getValeurs() {
	return _valeurs;
}

string Branche::getBrancheName() const {
	return _brancheName;
}

int Branche::getBrancheId() const {
	return _brancheId;
}