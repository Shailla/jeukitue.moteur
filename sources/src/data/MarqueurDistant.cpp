/*
 * MarqueurClient.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/MarqueurDistant.h"

MarqueurDistant::MarqueurDistant(Donnee* donnee, int donneeTmpId) {
	_donnee = donnee;
	_sentRevision = MARQUEUR_REVISION_INIT;
	_temporaryId = donneeTmpId;
}

MarqueurDistant::~MarqueurDistant() {
}

Donnee* MarqueurDistant::getDonnee() const {
	return _donnee;
}

int MarqueurDistant::getSentRevision(void) const {
	return _sentRevision;
}

void MarqueurDistant::setSentRevision(int sentRevision) {
	_sentRevision = sentRevision;
}

int MarqueurDistant::getTemporaryId() const {
	return _temporaryId;
}

void MarqueurDistant::setTemporaryId(int temporaryId) {
	_temporaryId = temporaryId;
}
