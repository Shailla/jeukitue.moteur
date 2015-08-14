/*
 * Donnee.cpp
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#include <stdexcept>

#include "data/MarqueurDistant.h"

#include "data/Branche.h"
#include "data/Valeur.h"

#include "data/Donnee.h"

using namespace std;

Donnee::Donnee(int revision, DONNEE_TYPE donneeType) {
	_revision = revision;
	_donneeType = donneeType;
}

Donnee::~Donnee() {
}

DONNEE_TYPE Donnee::getDonneeType() const {
	return _donneeType;
}

MarqueurDistant* Donnee::getMarqueur(DistantTreeProxy* distant) {
	MarqueurDistant* marqueur;

	try {
		marqueur = _marqueurs.at(distant);
	}
	catch(out_of_range& exception) {
		marqueur = NULL;
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
