/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>

using namespace std;

#include "util/types/IntData.h"
#include "data/ValeurInt.h"

using namespace JktUtils;

ValeurInt::ValeurInt(Branche* parent, int valeurId, const string& valeurName, int tmpId, int revision, int valeur) : Valeur(parent, valeurId, valeurName, tmpId, revision) {
	_valeur = valeur;
}

ValeurInt::~ValeurInt() {
}

int ValeurInt::getValeur() const {
	return _valeur;
}

Data* ValeurInt::getValeurData() const {
	return new JktUtils::IntData(_valeur);
}

void ValeurInt::setValeur(int revision, int valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurInt::updateValeur(int valeur) {
	_valeur = valeur;
	update();
}

void ValeurInt::setValeur(int revision, JktUtils::Data* data) {
	setRevision(revision);

	if(JktUtils::IntData* intData = dynamic_cast<JktUtils::IntData*>(data)) {
		_valeur = intData->getValue();
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
	}
}

