/*
 * MarqueurClient.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/MarqueurClient.h"

MarqueurClient::MarqueurClient(Donnee* donnee, int donneeTmpId) {
	_donnee = donnee;
	_sentRevision = MARQUEUR_REVISION_INIT;
	_temporaryId = donneeTmpId;
}

MarqueurClient::~MarqueurClient() {
}

Donnee* MarqueurClient::getDonnee() const {
	return _donnee;
}

int MarqueurClient::getSentRevision(void) const {
	return _sentRevision;
}

void MarqueurClient::setSentRevision(int sentRevision) {
	_sentRevision = sentRevision;
}

int MarqueurClient::getTemporaryId() const {
	return _temporaryId;
}

void MarqueurClient::setTemporaryId(int temporaryId) {
	_temporaryId = temporaryId;
}
