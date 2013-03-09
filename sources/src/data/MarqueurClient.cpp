/*
 * MarqueurClient.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/MarqueurClient.h"

MarqueurClient::MarqueurClient(Donnee* donnee, int donneeTmpId, bool isUpToDate) {
	_donnee = donnee;
	_isUpToDate = isUpToDate;
	_temporaryId = donneeTmpId;
}

MarqueurClient::~MarqueurClient() {
}

Donnee* MarqueurClient::getDonnee() const {
	return _donnee;
}

bool MarqueurClient::isUpToDate(void) const {
	return _isUpToDate;
}

int MarqueurClient::getTemporaryId() const {
	return _temporaryId;
}

void MarqueurClient::setTemporaryId(int temporaryId) {
	_temporaryId = temporaryId;
}
