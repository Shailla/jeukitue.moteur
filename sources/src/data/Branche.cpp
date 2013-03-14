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

Branche::Branche(Branche* parent, int brancheId, const string& brancheName) {
	_parent = parent;
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
		branche = NULL;
	}

	return branche;
}

Branche* Branche::createSubBranche(const string& brancheName) {
	// Alloue une référence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants démarrent à 0

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(this, ref, brancheName);
	_subBranches[ref] = newBranche;

	return newBranche;
}

Valeur* Branche::createValeurInt(const string& valeurName, int valeur) {
	// Alloue une référence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants démarrent à 0

	// Crée la nouvelle branche
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

Valeur* Branche::getValeur(int valeurId) {
	return _valeurs.at(valeurId);
}

string Branche::getBrancheName() const {
	return _brancheName;
}

int Branche::getBrancheId() const {
	return _brancheId;
}

void Branche::getBrancheFullId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullId(id);
	}

	id.push_back(_brancheId);
}

vector<int> Branche::getBrancheFullId() const {
	vector<int> id;
	getBrancheFullId(id);

	return id;
}
