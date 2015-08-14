/*
 * ValeurFloat.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

using namespace std;

#include "util/Trace.h"
#include "util/types/AnyData.h"

#include "data/ValeurFloat.h"

using namespace JktUtils;

ValeurFloat::ValeurFloat(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, int tmpId, int revision, float valeur) : Valeur(parent, valeurId, valeurName, valeurType, tmpId, revision) {
	_valeur = valeur;
}

ValeurFloat::~ValeurFloat() {
}

float ValeurFloat::getValeur() const {
	return _valeur;
}

AnyData ValeurFloat::getValeurData() const {
	return JktUtils::AnyData(_valeur);
}

void ValeurFloat::setValeur(int revision, float valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurFloat::updateValeur(float valeur) {
	_valeur = valeur;
	update();
}

void ValeurFloat::setValeur(int revision, const JktUtils::AnyData& data) {
	if(data.isFloat()) {
		setRevision(revision);
		_valeur = data.getValueFloat();
	}
	else {
		TRACE().error("Type de valeur inattendu");
	}
}

std::string ValeurFloat::toString() const {
	ostringstream str;
	str << "float-'" << _valeur << "'";
	return str.str();
}
