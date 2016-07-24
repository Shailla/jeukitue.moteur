/*
 * ValeurInt.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <typeinfo>

using namespace std;

#include "util/Trace.h"
#include "util/types/AnyData.h"

#include "data/ValeurInt.h"

using namespace jkt;

ValeurInt::ValeurInt(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, UPDATE_MODE updateMode, int tmpId, int revision, int valeur) : Valeur(parent, valeurId, valeurName, valeurType, updateMode, tmpId, revision) {
	_valeur = valeur;
}

ValeurInt::~ValeurInt() {
}

int ValeurInt::getValeur() const {
	return _valeur;
}

AnyData ValeurInt::getValeurData() const {
	return jkt::AnyData(_valeur);
}

void ValeurInt::setValeur(int revision, int valeur) {
	setRevision(revision);
	_valeur = valeur;
}

void ValeurInt::updateValeur(int valeur) {
	_valeur = valeur;
	update();
}

void ValeurInt::setValeur(int revision, const jkt::AnyData& data) {
	if(data.isInt()) {
		setRevision(revision);
		_valeur = data.getValueInt();
	}
	else {
		LOGERROR(("Type de valeur inattendu"));
	}
}

std::string ValeurInt::toString() const {
	ostringstream str;
	str << "int-'" << _valeur << "'";
	return str.str();
}
