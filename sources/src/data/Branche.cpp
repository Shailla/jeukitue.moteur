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

Branche::Branche(int brancheId) {
	_brancheId = brancheId;
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

Branche* Branche::createSubBranche() {
	// Alloue une référence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants démarrent à 0

	cout << endl << "REF:" << ref << flush;

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(ref);
	_subBranches[ref] = newBranche;

	return newBranche;
}

Valeur* Branche::createValeurInt(int valeur) {
	// Alloue une référence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants démarrent à 0

	// Crée la nouvelle branche
	Valeur* newValeur = new ValeurInt(ref);
	_valeurs[ref] = newValeur;

	return newValeur;
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
