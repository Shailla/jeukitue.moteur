/*
 * ValeurFloat.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

using namespace std;

#include "util/types/FloatData.h"
#include "data/ValeurFloat.h"

using namespace JktUtils;

ValeurFloat::ValeurFloat(Branche* parent, int valeurId, const string& valeurName, int tmpId, int revision, float valeur) : Valeur(parent, valeurId, valeurName, tmpId, revision) {
	_valeur = valeur;
}

ValeurFloat::~ValeurFloat() {
}

float ValeurFloat::getValeur() const {
	return _valeur;
}

Data* ValeurFloat::getValeurData() const {
	return new JktUtils::FloatData(_valeur);
}

void ValeurFloat::setValeur(int revision, float valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurFloat::updateValeur(float valeur) {
	_valeur = valeur;
	update();
}

void ValeurFloat::updateValeur(const JktUtils::Data* data) {
	try {
		const JktUtils::FloatData* stringData = dynamic_cast<const JktUtils::FloatData*>(data);
		_valeur = stringData->getValue();

		update();
	}
	catch(bad_cast& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type 'float' attendu";
	}
}

void ValeurFloat::setValeur(int revision, const JktUtils::Data& data) {
	try {
		const JktUtils::FloatData& intData = dynamic_cast<const JktUtils::FloatData&>(data);
		setRevision(revision);
		_valeur = intData.getValue();
	}
	catch(bad_cast& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type 'float' attendu";
	}
}

std::string ValeurFloat::toString() const {
	ostringstream str;
	str << _valeur;
	return str.str();
}
