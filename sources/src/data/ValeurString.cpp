/*
 * ValeurString.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

using namespace std;

#include "util/Trace.h"
#include "util/types/AnyData.h"

#include "data/ValeurString.h"

using namespace JktUtils;

ValeurString::ValeurString(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, int tmpId, int revision, const std::string& valeur) : Valeur(parent, valeurId, valeurName,  valeurType, tmpId, revision) {
	_valeur = valeur;
}

ValeurString::~ValeurString() {
}

const std::string& ValeurString::getValeur() const {
	return _valeur;
}

AnyData ValeurString::getValeurData() const {
	return JktUtils::AnyData(_valeur);
}

void ValeurString::setValeur(int revision, const std::string& valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurString::updateValeur(const std::string& valeur) {
	_valeur = valeur;
	update();
}

void ValeurString::setValeur(int revision, const JktUtils::AnyData& data) {
	if(data.isString()) {
		setRevision(revision);
		_valeur = data.getValueString();
	}
	else {
		TRACE().error("Type de valeur inattendu");
	}
}

std::string ValeurString::toString() const {
	ostringstream str;
	str << "string-'" << _valeur << "'";
	return str.str();
}

