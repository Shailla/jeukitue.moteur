/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Branche.h"

#include "data/Valeur.h"

Valeur::Valeur(Branche* parent, int valeurId, const string& valeurName) {
	_parent = parent;
	_valeurId = valeurId;
	_valeurName = valeurName;
}

Valeur::~Valeur() {
}

vector<int> Valeur::getBrancheId() const {
	return _parent->getBrancheFullId();
}

string Valeur::getValeurName() const {
	return _valeurName;
}

int Valeur::getValeurId() const {
	return _valeurId;
}
