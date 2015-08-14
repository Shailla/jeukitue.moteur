/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Branche.h"

#include "data/Valeur.h"

Valeur::Valeur(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, int valeurTmpId, int revision) : Donnee(revision, valeurType) {
	_parent = parent;
	_valeurId = valeurId;
	_valeurName = valeurName;
	_valeurTmpId = valeurTmpId;
}

Valeur::~Valeur() {
}

vector<int> Valeur::getBrancheId() const {
	return _parent->getBrancheFullId();
}

vector<int> Valeur::getBrancheIdOrTmpId() const {
	return _parent->getBrancheFullIdOrTmpId();
}

string Valeur::getValeurName() const {
	return _valeurName;
}

int Valeur::getValeurTmpId() const {
	return _valeurTmpId;
}

int Valeur::getValeurId() const {
	return _valeurId;
}

void Valeur::setValeurId(int valeurId) {
	_valeurId = valeurId;
}

bool Valeur::highestId(const Valeur* left, const Valeur* right) {
	int leftId = left->getValeurId();
	int rightId = right->getValeurId();

	return leftId < rightId;
}
