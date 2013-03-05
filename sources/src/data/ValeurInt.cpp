/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/ValeurInt.h"

ValeurInt::ValeurInt() {
	_valeur = 0;
}

ValeurInt::~ValeurInt() {
}

int ValeurInt::getInt() const {
	return _valeur;
}

