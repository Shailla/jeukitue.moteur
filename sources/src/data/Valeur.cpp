/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Valeur.h"

Valeur::Valeur(int valeurId, const string& valeurName) {
	_valeurId = valeurId;
	_valeurName = valeurName;
}

Valeur::~Valeur() {
}

string Valeur::getValeurName() const {
	return _valeurName;
}

int Valeur::getValeurId() const {
	return _valeurId;
}

