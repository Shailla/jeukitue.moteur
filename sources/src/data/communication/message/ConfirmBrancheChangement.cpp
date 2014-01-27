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

#include "data/communication/message/ConfirmBrancheChangement.h"

ConfirmBrancheChangement::ConfirmBrancheChangement(istringstream& in) : Changement("ConfirmBrancheChangement", PRIORITY_ConfirmBrancheChangement) {
	unserialize(in);
}

ConfirmBrancheChangement::ConfirmBrancheChangement(const vector<int>& brancheId, int revision) : Changement("ConfirmBrancheChangement", PRIORITY_ConfirmBrancheChangement) {
	_brancheId = brancheId;
	_revision = revision;
}

ConfirmBrancheChangement::~ConfirmBrancheChangement() {
};

void ConfirmBrancheChangement::update(MarqueurDistant* marqueur) {
	// Met à jour l'état des données
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void ConfirmBrancheChangement::serialize(ostringstream& out) const {
	// Serialize
	StreamUtils::write(out, (int)CONFIRM_BRANCHE_MESSAGE);

	StreamUtils::write(out, _brancheId);
	StreamUtils::write(out, _revision);
}

void ConfirmBrancheChangement::unserialize(istringstream& in) {
	StreamUtils::read(in, _brancheId);
	StreamUtils::read(in, _revision);
}

string ConfirmBrancheChangement::toString() const {
	ostringstream str;

	str << "[" << _dataType;

	str << "; brancheId:";
	StreamUtils::writeHumanReadable(str, _brancheId);

	str << "; revision:" << _revision;

	str << "]\t" << Changement::toString();

	return str.str();
}

const std::vector<int>& ConfirmBrancheChangement::getBrancheId() const {
	return _brancheId;
}

int ConfirmBrancheChangement::getRevision() const {
	return _revision;
}

int ConfirmBrancheChangement::getRootDistance() const {
	return _brancheId.size();
}
