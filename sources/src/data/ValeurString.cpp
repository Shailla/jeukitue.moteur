/*
 * ValeurString.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

using namespace std;

#include "util/types/StringData.h"
#include "data/ValeurString.h"

using namespace JktUtils;

ValeurString::ValeurString(Branche* parent, int valeurId, const string& valeurName, int tmpId, int revision, const std::string& valeur) : Valeur(parent, valeurId, valeurName, tmpId, revision) {
	_valeur = valeur;
}

ValeurString::~ValeurString() {
}

const std::string& ValeurString::getValeur() const {
	return _valeur;
}

Data* ValeurString::getValeurData() const {
	return new JktUtils::StringData(_valeur);
}

void ValeurString::setValeur(int revision, const std::string& valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurString::updateValeur(const std::string& valeur) {
	_valeur = valeur;
	update();
}

void ValeurString::setValeur(int revision, const JktUtils::Data& data) {
	try {
		const JktUtils::StringData& stringData = dynamic_cast<const JktUtils::StringData&>(data);
		setRevision(revision);
		_valeur = stringData.getValue();
	}
	catch(bad_cast& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type 'int' attendu";
	}
}

