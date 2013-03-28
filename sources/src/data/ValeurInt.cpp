/*
 * Valeur.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

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

void ValeurInt::setValeur(int revision, const JktUtils::Data& data) {
	try {
		const JktUtils::IntData& intData = dynamic_cast<const JktUtils::IntData&>(data);
		setRevision(revision);
		_valeur = intData.getValue();
	}
	catch(bad_cast& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type 'int' attendu";
	}
}

