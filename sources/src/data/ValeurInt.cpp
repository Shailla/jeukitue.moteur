/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/ValeurInt.h"

ValeurInt::ValeurInt(int valeurId, const string& valeurName, int valeur) : Valeur(valeurId, valeurName) {
	_valeur = valeur;
}

ValeurInt::~ValeurInt() {
}

int ValeurInt::getInt() const {
	return _valeur;
}

