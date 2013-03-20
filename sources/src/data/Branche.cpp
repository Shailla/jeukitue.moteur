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

#include "util/types/IntData.h"
#include "data/ValeurInt.h"

#include "data/Branche.h"

Branche::Branche(Branche* parent, int brancheId, const string& brancheName) {
	_parent = parent;
	_brancheId = brancheId;
	_brancheName = brancheName;
}

Branche::Branche(Branche* parent, int brancheId, const string& brancheName, int revision): Donnee(revision) {
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


Branche* Branche::addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision) {
	if(_subBranches.find(brancheId) != _subBranches.end()) {
		cerr << endl << "La branche existe deja";
	}

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(this, brancheId, brancheName, brancheRevision);
	_subBranches[brancheId] = newBranche;

	return newBranche;
}


Valeur* Branche::createValeurInt(const string& valeurName, int valeur) {
	// Alloue une référence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants démarrent à 0

	// Crée la nouvelle branche
	Valeur* newValeur = new ValeurInt(this, ref, valeurName, valeur);
	_valeurs[ref] = newValeur;

	return newValeur;
}

//Valeur* Branche::addValeurInt(int valeurId, const string& valeurName, int valeurRevision, int valeur) {
//	if(_valeurs.find(valeurId) != _valeurs.end()) {
//		cerr << endl << "La valeur existe deja";
//	}
//
//	// Crée la nouvelle valeur
//	Valeur* newValeur = new ValeurInt(this, valeurId, valeurName, valeur);
//	_valeurs[valeurId] = newValeur;
//}

Valeur* Branche::addValeurInt(int valeurId, const string& valeurName, int valeurRevision, JktUtils::Data* valeur) {
	Valeur* newValeur = NULL;

	if(_valeurs.find(valeurId) != _valeurs.end()) {
		cerr << endl << "La valeur existe deja";
	}

	// Crée la nouvelle valeur
	if(JktUtils::IntData* intData = dynamic_cast<JktUtils::IntData*>(valeur)) {
		Valeur* newValeur = new ValeurInt(this, valeurId, valeurName, intData->getValue());
		_valeurs[valeurId] = newValeur;
	}
	else {
		cerr << endl << "Type de valeur inconnu";
	}

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


vector<int> Branche::getParentBrancheId(void) const {
	vector<int> parentBrancheId;

	if(_parent) {
		parentBrancheId = _parent->getBrancheFullId();
	}
	else {
		// Do nothing, return empty id
	}

	return parentBrancheId;
}

void Branche::getBrancheFullId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullId(id);
		id.push_back(_brancheId);
	}
}

vector<int> Branche::getBrancheFullId() const {
	vector<int> id;
	getBrancheFullId(id);

	return id;
}
