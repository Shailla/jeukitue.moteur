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
#include "data/MarqueurDistant.h"

#include "data/communication/message/ConfirmValeurChangement.h"

ConfirmValeurChangement::ConfirmValeurChangement(istringstream& in) : Changement("ConfirmValeurChangement", PRIORITY_ConfirmValeurChangement) {
	unserialize(in);
}

ConfirmValeurChangement::ConfirmValeurChangement(const vector<int>& brancheId, int valeurId, int revision) : Changement("ConfirmValeurChangement", PRIORITY_ConfirmValeurChangement) {
	_brancheId = brancheId;
	_valeurId = valeurId;
	_revision = revision;
}

ConfirmValeurChangement::~ConfirmValeurChangement() {
};

void ConfirmValeurChangement::update(MarqueurDistant* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void ConfirmValeurChangement::serialize(ostringstream& out) const {
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

string ConfirmValeurChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << "; brancheId:";
	StreamUtils::writeHumanReadable(str, _brancheId);

	str << "; valeurId:" << _valeurId;

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
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

int ConfirmValeurChangement::getRootDistance() const {
	return _brancheId.size();
}
