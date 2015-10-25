/*
 * Donnee.cpp
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <stdexcept>

#include "data/MarqueurDistant.h"
#include "util/CollectionsUtils.h"
#include "data/Branche.h"
#include "data/Valeur.h"

#include "data/Donnee.h"

using namespace std;

Donnee::Donnee(DataTree* dataTree, int revision, DONNEE_TYPE donneeType, UPDATE_MODE updateMode) {
	_dataTree = dataTree;
	_revision = revision;
	_donneeType = donneeType;
	_updateMode = updateMode;
}

Donnee::~Donnee() {
}

DONNEE_TYPE Donnee::getDonneeType() const {
	return _donneeType;
}

UPDATE_MODE Donnee::getUpdateMode() const {
	return _updateMode;
}

DataTree* Donnee::getDataTree() const {
	return _dataTree;
}

MarqueurDistant* Donnee::getMarqueur(DistantTreeProxy* distant) {
	MarqueurDistant* marqueur;

	try {
		marqueur = _marqueurs.at(distant);
	}
	catch(out_of_range& exception) {
		marqueur = 0;
		cout << endl << "ahhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh MARQUEUR INTROUVABLE" << flush;
//		cout << endl << "==> " << JktUtils::CollectionsUtils::toString(((Valeur*)this)->getValeurFullId()) << flush;

		if(dynamic_cast<Branche*>(this)) {
			cout << endl << "==> BR " << ((Branche*)this)->getBrancheName() << flush;
		}
		else {
			cout << endl << "==> VL " << ((Valeur*)this)->getValeurName() << flush;
		}
	}

	return marqueur;
}

MarqueurDistant* Donnee::addMarqueur(DistantTreeProxy* distant, int donneeTmpId) {
	MarqueurDistant* marqueur = new MarqueurDistant(this, donneeTmpId);
	_marqueurs[distant] = marqueur;

	return marqueur;
}

void Donnee::update() {
	_revision++;
}

void Donnee::setRevision(int revision) {
	_revision = revision;
}

int Donnee::getRevision(void) const {
	return _revision;
}
