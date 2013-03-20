/*
 * ConfirmValeurChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

#include "util/StreamUtils.h"
#include "util/types/IntData.h"
#include "data/ValeurInt.h"
#include "data/ServeurDataTree.h"

#include "data/communication/message/ConfirmValeurChangement.h"

ConfirmValeurChangement::ConfirmValeurChangement(istringstream& in) {
	unserialize(in);
}

ConfirmValeurChangement::ConfirmValeurChangement(Valeur* valeur) {
	_brancheId = valeur->getBrancheId();
	_valeurId = valeur->getValeurId();
	_revision = valeur->getRevision();
}

ConfirmValeurChangement::~ConfirmValeurChangement() {
};

void ConfirmValeurChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void ConfirmValeurChangement::serialize(ostringstream& out) {
	// Serialize
	StreamUtils::write(out, (int)CONFIRM_VALEUR_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _valeurId);
	StreamUtils::write(out, _revision);
}

void ConfirmValeurChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _valeurId);
	StreamUtils::read(in, _revision);
}

const std::vector<int>& ConfirmValeurChangement::getBrancheId() const {
	return _brancheId;
}

int ConfirmValeurChangement::getValeurId() const {
	return _valeurId;
}

int ConfirmValeurChangement::getRevision() const {
	return _revision;
}
